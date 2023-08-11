#include "BxDecay0Generator.hh"
//_________________________________________________________________________________________
#include <iostream>
#include <random>
#include <limits>
//_________________________________________________________________________________________
#include <bxdecay0/decay0_generator.h>
#include "bxdecay0/std_random.h"
#include "bxdecay0/event.h"  
#include "bxdecay0/bb_utils.h"
#include "bxdecay0/mdl_event_op.h"
//_________________________________________________________________________________________
#include <globals.hh>
#include <G4ParticleMomentum.hh>
#include <G4ParticleGun.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4Alpha.hh>
#include <G4SystemOfUnits.hh>
#include <G4RunManager.hh>
//_________________________________________________________________________________________
#include "BxDecay0Messenger.hh"
//_________________________________________________________________________________________
namespace {
  struct RadiologicalGun : public G4ParticleGun {
    friend class HitDataHandler;
  public:
    RadiologicalGun() = default;
    RadiologicalGun(G4int number_of_particles) : G4ParticleGun(number_of_particles) {}
    RadiologicalGun(G4ParticleDefinition* particle_def, G4int number_of_particles = 1) : G4ParticleGun(particle_def, number_of_particles) {}
    virtual ~RadiologicalGun() override = default;
    RadiologicalGun(const RadiologicalGun&) = delete;
    const RadiologicalGun& operator=(const RadiologicalGun&) = delete;
    G4bool operator==(const RadiologicalGun&) const = delete;
    G4bool operator!=(const RadiologicalGun&) const = delete;
    void ResetParticleData() {
      NumberOfParticlesToBeGenerated = 1;
      particle_definition = nullptr;
      G4ThreeVector zero;
      particle_momentum_direction = (G4ParticleMomentum)zero;
      particle_energy = 0.0;
      particle_momentum = 0.0;
      particle_position = zero;
      particle_time = 0.0;
      particle_polarization = zero;
      particle_charge = 0.0;
    }
  };
}
//_________________________________________________________________________________________
namespace BxDecay0Generator {
  void PrimaryGeneratorAction::ConfigurationInterface::print(std::ostream & out_, const std::string & indent_) const {
    out_ << indent_ << "Configuration interface: \n";
    out_ << indent_ << "|-- Decay category : '" << decay_category << "'\n";
    out_ << indent_ << "|-- Nuclide : '" << nuclide << "'\n";
    out_ << indent_ << "|-- Seed : " << seed << "\n";
    if (decay_category == "dbd") {
      out_ << indent_ << "|-- DBD mode : " << dbd_mode << "\n";
      out_ << indent_ << "|-- DBD level : " << dbd_level << "\n";
      out_ << indent_ << "|-- DBD min energy (MeV) : " << dbd_min_energy_MeV << "\n";
      out_ << indent_ << "|-- DBD max energy (MeV) : " << dbd_max_energy_MeV << "\n";
    }
    out_ << indent_ << "|-- Debug : " << std::boolalpha << debug << "\n";
    out_ << indent_ << "`-- Use MDL : " << std::boolalpha << use_mdl << "\n";
    if (use_mdl) {
      out_ << indent_ << "    " << "|-- Target particle name : " << mdl_target_name << "\n";
      out_ << indent_ << "    " << "|-- Target particle rank : " << mdl_target_rank << "\n";
      out_ << indent_ << "    " << "|-- Cone longitude       : " << mdl_cone_longitude << " degrees\n";
      out_ << indent_ << "    " << "|-- Cone colatitude      : " << mdl_cone_colatitude << " degrees\n";
      out_ << indent_ << "    " << "|-- Cone aperture        : " << mdl_cone_aperture << " degrees\n";
      out_ << indent_ << "    " << "|-- Cone aperture 2      : " << mdl_cone_aperture2 << " degrees\n";
      out_ << indent_ << "    " << "`-- Error on missing target particle : " << std::boolalpha << mdl_error_on_missing_particle << "\n";
    }
    return;
  }
  //_____________________________________________________________________________________
  bool PrimaryGeneratorAction::ConfigurationInterface::is_valid_base() const {
    if (decay_category != "background" and decay_category != "dbd") {
      return false;
    }
    if (nuclide.empty()) {
      return false;
    }
    if (seed < 1) {
      return false;
    }
    if (decay_category == "dbd") {
      if (dbd_mode < 1) {
        return false;
      }
      if (dbd_level < 0) { 
        return false;
      }
    }
    return true;
  }
  //_____________________________________________________________________________________
  bool PrimaryGeneratorAction::ConfigurationInterface::is_valid_mdl() const {
    if (use_mdl) {
      if (mdl_target_name.empty()) return false;
      if (mdl_target_rank < -1) return false;
    }
    return true;
  }
  //_____________________________________________________________________________________
  bool PrimaryGeneratorAction::ConfigurationInterface::is_valid() const {
    return is_valid_base() and is_valid_mdl();
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::ConfigurationInterface::reset_base() {
    decay_category = "";
    nuclide = "";
    seed = 1;
    dbd_mode = 0;
    dbd_level = 0;
    dbd_min_energy_MeV = -1.0;
    dbd_max_energy_MeV = -1.0;
    debug = false;
    return;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::ConfigurationInterface::reset_mdl() {
    use_mdl = false;
    mdl_target_name = "";
    mdl_target_rank = -1;
    mdl_cone_longitude  = 0.0;
    mdl_cone_colatitude = 0.0;
    mdl_cone_aperture   = 0.0;
    mdl_cone_aperture2  = -1.0; 
    mdl_error_on_missing_particle = false;
    return;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::ConfigurationInterface::reset() {
    reset_base();
    reset_mdl();
    return;
  }
  //_____________________________________________________________________________________
  struct Configuration {
    bxdecay0::decay0_generator::decay_category_type decay_category = bxdecay0::decay0_generator::DECAY_CATEGORY_UNDEFINED;
    std::string   nuclide;      
    unsigned int  seed = 314159; 
    int           dbd_level = 0;  
   
    bxdecay0::dbd_mode_type dbd_mode = bxdecay0::DBDMODE_UNDEF; 

    double dbd_min_energy_MeV = std::numeric_limits<double>::quiet_NaN();
    double dbd_max_energy_MeV = std::numeric_limits<double>::quiet_NaN();
    bool   debug = false; ///< Debug flag
  };
  //_____________________________________________________________________________________
  struct MdlEventOpConfiguration {
    bool   use_mdl = false; 
    bxdecay0::particle_code code = bxdecay0::INVALID_PARTICLE; 
    int    rank = -1; 
    double cone_phi = 0.0;
    double cone_theta = 0.0; 
    double cone_aperture = 0.0; 
    double cone_aperture2 = std::numeric_limits<double>::quiet_NaN(); 
    bool   error_on_missing_particle = false; 
  };
  //_____________________________________________________________________________________
  struct PrimaryGeneratorAction::pimpl_type {
    pimpl_type(PrimaryGeneratorAction * action);
    ~pimpl_type();
    bxdecay0::decay0_generator & get_decay0();
    void destroy_decay0();
    void destroy_generator();
    void destroy_prng();
    void destroy();

    std::default_random_engine & get_generator();
    bxdecay0::std_random & get_prng();

    PrimaryGeneratorAction *     action = nullptr;     
    Configuration                config;              
    MdlEventOpConfiguration      mdl_config;          
    std::default_random_engine * pgenerator = nullptr; 
    bxdecay0::std_random *       pprng = nullptr;     
    bxdecay0::decay0_generator * pdecay0 = nullptr; 
  };
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::pimpl_type::destroy() {
    destroy_decay0();
    destroy_prng();
    destroy_generator();
    return;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::pimpl_type::destroy_decay0() {
    if (action->IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::destroy_decay0: Entering...\n";
    if (pdecay0 != nullptr) {
      if (action->IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::destroy_decay0: Destroying the BxDecay0 driver...\n";
      pdecay0->reset();
      delete pdecay0;
      pdecay0 = nullptr;
    }
    if (action->IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::destroy_decay0: Exiting...\n";
    return;
  }
  //_____________________________________________________________________________________
  bxdecay0::decay0_generator & PrimaryGeneratorAction::pimpl_type::get_decay0() {
    if (action->IsTrace()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_decay0: Entering...\n";
    if (action->ConfigHasChanged()) {
      if (action->IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_decay0: Configuration has changed\n";
      if (action->GetConfiguration().is_valid()) {
        action->ApplyConfiguration();
        if (pdecay0 != nullptr) {
          if (action->IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_decay0: Destroying the current BxDecay0 generator instance because of a new config...\n";
          destroy();
        }
      } else {
        if (action->IsTrace()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_decay0: Invalid configuration!\n";
        
      }
    }
    if (pdecay0 == nullptr) {
      if (action->IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_decay0: Instantiating and configuring a new BxDecay0 generator...\n";
      pdecay0 = new bxdecay0::decay0_generator;
      pdecay0->set_debug(config.debug);
      pdecay0->set_decay_category(config.decay_category);
      pdecay0->set_decay_isotope(config.nuclide);
      double toallevents = 1.0;
      bool use_specific_erange = false;
      if (config.decay_category == bxdecay0::decay0_generator::DECAY_CATEGORY_DBD) {
        pdecay0->set_decay_dbd_level(config.dbd_level);
        pdecay0->set_decay_dbd_mode(config.dbd_mode);
        if (! std::isnan(config.dbd_min_energy_MeV) or !std::isnan(config.dbd_max_energy_MeV)) {
          double emin_MeV = 0.0;
          double emax_MeV = 5000.0;
          if (! std::isnan(config.dbd_min_energy_MeV)) {
            emin_MeV = config.dbd_min_energy_MeV;
          }
          if (! std::isnan(config.dbd_max_energy_MeV)) {
            emax_MeV = config.dbd_max_energy_MeV;
          }
          pdecay0->set_decay_dbd_esum_range(emin_MeV, emax_MeV);
          use_specific_erange = true;
        }
      }
      if (mdl_config.use_mdl) {
        if (action->IsDebug()) {
          std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_decay0: "
                    << "Install the MDL post-generation operation in the BxDecay0 generator...\n";
        }
        bxdecay0::event_op_ptr mdlPtr(new bxdecay0::momentum_direction_lock_event_op(pdecay0->is_debug()));
        bxdecay0::momentum_direction_lock_event_op & mdl = dynamic_cast<bxdecay0::momentum_direction_lock_event_op&>(*mdlPtr);
        if (mdl_config.cone_aperture2 >= 0.0) {
          mdl.set_with_aperture_rectangular_cut(mdl_config.code, 
                                                mdl_config.rank,               
                                                mdl_config.cone_phi,
                                                mdl_config.cone_theta,   
                                                mdl_config.cone_aperture,        
                                                mdl_config.cone_aperture2,        
                                                mdl_config.error_on_missing_particle);
        } else {
          mdl.set(mdl_config.code, 
                  mdl_config.rank,               
                  mdl_config.cone_phi,
                  mdl_config.cone_theta,   
                  mdl_config.cone_aperture,        
                  mdl_config.error_on_missing_particle);
        }
        pdecay0->add_operation(mdlPtr); 
      }
      if (action->IsDebug()) {
        pdecay0->smart_dump(std::cerr,"BxDecay0 Generator: ", "[debug] ");
      }
      pdecay0->initialize(get_prng());
      if (use_specific_erange) {
        toallevents = pdecay0->get_bb_params().toallevents;
        if (action->IsInfo()) std::cerr << "[info] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_decay0: BxDecay0 Geant4 extension library: toallevents=" << toallevents << "\n";
      }
    }
    return *pdecay0;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::pimpl_type::destroy_prng() {
    if (pprng != nullptr) {
      if (pdecay0 != nullptr) destroy_decay0();
      if (action->IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::destroy_prng: Destroying the high level random generator...\n";
      delete pprng;
      pprng = nullptr;
    }
    return;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::pimpl_type::destroy_generator() {
    if (pgenerator != nullptr) {
      if (pprng != nullptr) destroy_prng();
      if (action->IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::destroy_generator: Destroying the low level random generator...\n";
      delete pgenerator;
      pgenerator = nullptr;
    }
    return;
  }
  //_____________________________________________________________________________________
  std::default_random_engine & PrimaryGeneratorAction::pimpl_type::get_generator() {
    if (pgenerator == nullptr) {
      if (action->IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_generator: Instantiating a new low level random generator with seed=" << config.seed << "...\n";
      pgenerator = new std::default_random_engine(config.seed);
    }
    return *pgenerator;
  }
  //_____________________________________________________________________________________
  bxdecay0::std_random & PrimaryGeneratorAction::pimpl_type::get_prng() {
    if (pprng == nullptr) {
      if (action->IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_prng: Instantiating a new high level random generator...\n";
      pprng = new bxdecay0::std_random(get_generator());
    }
    return *pprng;
  }
  //_____________________________________________________________________________________
  PrimaryGeneratorAction::pimpl_type::pimpl_type(PrimaryGeneratorAction * action_) {
    action = action_;
    if (action->IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::ctor: Construction of the BxDecay0 Geant4 Plugin PIMPL...\n";
    return;
  }
  //_____________________________________________________________________________________
  PrimaryGeneratorAction::pimpl_type::~pimpl_type() {
    destroy();
    if (action->IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::dtor: Destruction of the BxDecay0 Geant4 Plugin PIMPL.\n";
    return;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::SetVerbosity(int level_) {
    _verbosity_ = level_ < 0 ? 0 : level_;
    return;
  }
  //_____________________________________________________________________________________
  int PrimaryGeneratorAction::GetVerbosity() const {
    return _verbosity_;
  }
  //_____________________________________________________________________________________
  bool PrimaryGeneratorAction::IsInfo() const {
    return _verbosity_ > 0;
  }
  //_____________________________________________________________________________________
  bool PrimaryGeneratorAction::IsDebug() const {
    return _verbosity_ > 1;
  }
  //_____________________________________________________________________________________
  bool PrimaryGeneratorAction::IsTrace() const {
    return _verbosity_ > 2;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::Dump(std::ostream & out_) const {
    out_ << "=== BxDecay0Generator::PrimaryGeneratorAction ===\n";
    out_ << "Verbosity : " << _verbosity_ << "\n";
    out_ << "Vertex generator : " << (_vertex_generator_ ? "yes" : "no") << "\n";
    out_ << "Messenger : " << (_messenger_ ? "yes" : "no") << "\n";
    _config_.print(out_);
    out_ << "Configuration has changed : " << std::boolalpha << _config_has_changed_ << "\n";
    out_ << "PIMPL : " << (_pimpl_ ? "yes" : "no") << "\n";
    if (_pimpl_ and _pimpl_->pdecay0) {
      _pimpl_->pdecay0->smart_dump(out_, "Decay0 generator instance: ", "");
    }
    return;
  }
  //_____________________________________________________________________________________ 
  PrimaryGeneratorAction::PrimaryGeneratorAction(int verbosity_) : G4VUserPrimaryGeneratorAction(), _particle_gun_(nullptr) {
    bulk_vertex_generator_ = new BulkVertexGenerator();
    SetVerbosity(verbosity_);
    if (IsDebug()) std::cerr << "[debug] Instantiating BxDecay0 Geant4 Plugin PIMPL...\n";
    _pimpl_.reset(new pimpl_type(this));
    if (IsDebug()) std::cerr << "[debug] Instantiating Geant4 particle gun...\n";
    _particle_gun_ = new RadiologicalGun(1);
    if (IsDebug()) std::cerr << "[debug] Instantiating messenger...\n";
    _messenger_ = new PrimaryGeneratorActionMessenger(this);
    return;
  }
  //_____________________________________________________________________________________
  PrimaryGeneratorAction::PrimaryGeneratorAction(const ConfigurationInterface & config_inter_, int verbosity_) : PrimaryGeneratorAction(verbosity_) {
    bulk_vertex_generator_ = new BulkVertexGenerator();
    SetConfiguration(config_inter_);
    return;
  }
  //_____________________________________________________________________________________
  PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    if (_owned_vertex_generator_ and _vertex_generator_ != nullptr) {
      if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::~PrimaryGeneratorAction: Destroying the vertex generator...\n";
     delete _vertex_generator_;
      _vertex_generator_ = nullptr;
    }
    if (_messenger_) {
      if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::~PrimaryGeneratorAction: Terminating messenger...\n";
      delete _messenger_;
    }
    if (_particle_gun_) {
      if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::~PrimaryGeneratorAction: Terminating Geant4 particle gun...\n";
      delete _particle_gun_;
    }
    if (_pimpl_) {
      if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::~PrimaryGeneratorAction: Terminating BxDecay0 Geant4 Plugin PIMPL stuff...\n";
      _pimpl_.reset();
    }
    return;
  }
  //_____________________________________________________________________________________
  const PrimaryGeneratorAction::ConfigurationInterface & PrimaryGeneratorAction::GetConfiguration() const {
    return _config_;
  }
  //_____________________________________________________________________________________
  PrimaryGeneratorAction::ConfigurationInterface & PrimaryGeneratorAction::GrabConfiguration() {
    if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::GrabConfiguration: Destroy PIMPL material, if any...\n";
    return _config_;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::SetConfigHasChanged(bool changed_) {
    if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::SetConfigHasChanged: changed=" << changed_ << "\n";
    _config_has_changed_ = changed_;
    return;
  }
  //_____________________________________________________________________________________
  bool PrimaryGeneratorAction::ConfigHasChanged() const {
    return _config_has_changed_;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::SetConfiguration(const ConfigurationInterface & config_inter_) {
    if (IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::SetConfiguration: Entering...\n";
    _config_ = config_inter_;
    SetConfigHasChanged(true);
    if (IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::SetConfiguration: Exiting...\n";
    return;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::DestroyConfiguration() {
    if (IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::DestroyConfiguration: Entering...\n";
    if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::DestroyConfiguration: Entering...\n";
    _pimpl_->destroy();
    _config_.reset();
    _config_has_changed_ = false;
    if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::DestroyConfiguration: Exiting...\n";
    if (IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::DestroyConfiguration: Exiting...\n";
    return;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::ApplyConfiguration() {
    if (IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Entering...\n";
    if (not _config_.is_valid()) {
      std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Invalid configuration!\n";
      return;
    }
    bool error = false;
    if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::pimpl_type::get_decay0: Destroying the old BxDecay0 generator because of a new config to be applied...\n";
    _pimpl_->destroy();
    {
      Configuration dummyConfig;
      _pimpl_->config = dummyConfig;
      MdlEventOpConfiguration dummyMdlDummy;
      _pimpl_->mdl_config = dummyMdlDummy;
    };
    
    _pimpl_->config.debug = _config_.debug;

    if (not error) {
      if (_config_.seed <= 0) {
        std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Invalid seed value " << _config_.seed << "!\n";
        error = true;
      } else {
        _pimpl_->config.seed = _config_.seed;
      }
    }

    if (not error) {
      if (_config_.decay_category == "dbd") {
        _pimpl_->config.decay_category = bxdecay0::decay0_generator::DECAY_CATEGORY_DBD;
      } else if (_config_.decay_category == "background") {
        _pimpl_->config.decay_category = bxdecay0::decay0_generator::DECAY_CATEGORY_BACKGROUND;
      } else {
        std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Unsupported decay category '" << _config_.decay_category << "'!\n";
        error = true;
      }
    }

    if (not error) {
      if (_pimpl_->config.decay_category == bxdecay0::decay0_generator::DECAY_CATEGORY_DBD) {
        if (bxdecay0::dbd_isotopes().count(_config_.nuclide) == 0) {
          std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Unsupported DBD nuclide '" << _config_.nuclide << "'!\n";
          error = true;
        } 
      } else if (_pimpl_->config.decay_category == bxdecay0::decay0_generator::DECAY_CATEGORY_DBD) {
        if (bxdecay0::background_isotopes().count(_config_.nuclide) == 0) {
          std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Unsupported background nuclide '" << _config_.nuclide << "'!\n";
          error = true;
        } 
      }
      if (not error) {
        _pimpl_->config.nuclide = _config_.nuclide;
      }
    }

    if (not error and _pimpl_->config.decay_category == bxdecay0::decay0_generator::DECAY_CATEGORY_DBD) {
      if (_config_.dbd_mode < bxdecay0::DBDMODE_MIN or _config_.dbd_mode > bxdecay0::DBDMODE_MAX) {
        std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Invalid DBD decay mode " << _config_.dbd_mode << "!";
        error = true;
      } else {
        _pimpl_->config.dbd_mode = static_cast<bxdecay0::dbd_mode_type>(_config_.dbd_mode);
      }
      if (not error) {
        if (_config_.dbd_level < 0) {
          std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Invalid DBD daughter level index " << _config_.dbd_level << "!";
          error = true;
        } else {
          _pimpl_->config.dbd_level = (int) _config_.dbd_level;
        }
      }
      if (not error) {
        if (_config_.dbd_min_energy_MeV > 0.0) {
          _pimpl_->config.dbd_min_energy_MeV = (double) _config_.dbd_min_energy_MeV;
        }
        if (_config_.dbd_max_energy_MeV > 0.0) {
          _pimpl_->config.dbd_max_energy_MeV = (double) _config_.dbd_max_energy_MeV;
          if (_config_.dbd_min_energy_MeV > 0.0) {
            if (_pimpl_->config.dbd_min_energy_MeV >= _pimpl_->config.dbd_max_energy_MeV) {
              std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Invalid DBD energy range!";
              error = true;
            }
          }
        }
      }
    }

    if (_config_.use_mdl) {
      MdlEventOpConfiguration & mdlConfig = _pimpl_->mdl_config;
      if (not error) {
        mdlConfig.use_mdl = _config_.use_mdl;
        if (_config_.mdl_target_name == "*" or _config_.mdl_target_name == "all") {
          mdlConfig.code = bxdecay0::INVALID_PARTICLE; 
        } else if (_config_.mdl_target_name == "e-" or _config_.mdl_target_name == "electron") {
          mdlConfig.code = bxdecay0::ELECTRON;
        } else if (_config_.mdl_target_name == "e+" or _config_.mdl_target_name == "positron") {
          mdlConfig.code = bxdecay0::POSITRON;
        } else if (_config_.mdl_target_name == "g" or _config_.mdl_target_name == "gamma") {
          mdlConfig.code = bxdecay0::GAMMA;
        } else if (_config_.mdl_target_name == "n" or _config_.mdl_target_name == "neutron") {
          mdlConfig.code = bxdecay0::NEUTRON;
        } else if (_config_.mdl_target_name == "p" or _config_.mdl_target_name == "proton") {
          mdlConfig.code = bxdecay0::PROTON;
        } else if (_config_.mdl_target_name == "a" or _config_.mdl_target_name == "alpha") {
          mdlConfig.code = bxdecay0::ALPHA;
        } else {
          std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Invalid target particle label '" << _config_.mdl_target_name << "'!";
          error = true;
        }
      }
      
      if (not error) {     
        if (_config_.mdl_target_rank < 0) {
          mdlConfig.rank = -1; 
        } else {
          mdlConfig.rank = _config_.mdl_target_rank; 
        }
      }
      
      if (not error) {
        mdlConfig.cone_phi = _config_.mdl_cone_longitude * M_PI / 180.0;
        mdlConfig.cone_theta = _config_.mdl_cone_colatitude * M_PI / 180.0;
        mdlConfig.cone_aperture = _config_.mdl_cone_aperture * M_PI / 180.0;
        mdlConfig.cone_aperture2 = std::numeric_limits<double>::quiet_NaN();
        if (_config_.mdl_cone_aperture2 >= 0.0) {
          mdlConfig.cone_aperture2 = _config_.mdl_cone_aperture2 * M_PI / 180.0;
        }
        mdlConfig.error_on_missing_particle = _config_.mdl_error_on_missing_particle;
      }
    }
    
    if (error) {
      std::cerr << "[error] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Invalid configuration! Abort run!\n";
      G4RunManager::GetRunManager()->AbortRun();
    }
    SetConfigHasChanged(false);
    if (IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::ApplyConfiguration: Exiting...\n";
    return;
  }
  //_____________________________________________________________________________________
  G4ParticleGun * PrimaryGeneratorAction::GetParticleGun() {
    return _particle_gun_;
  }
  //_____________________________________________________________________________________
  bool PrimaryGeneratorAction::HasVertexGenerator() const {
    return _vertex_generator_ != nullptr;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::SetVertexGenerator(VertexGeneratorInterface & vertex_generator_) {
    if (_owned_vertex_generator_ and _vertex_generator_ != nullptr) {
      delete _vertex_generator_;
      _vertex_generator_ = nullptr;
      _owned_vertex_generator_ = false;
    }
    _vertex_generator_ = &vertex_generator_;
    _owned_vertex_generator_ = false;
    return;
  }
  //_____________________________________________________________________________________
  void PrimaryGeneratorAction::SetVertexGenerator(VertexGeneratorInterface * vertex_generator_ptr_) {
    if (_owned_vertex_generator_ and _vertex_generator_ != nullptr) {
      delete _vertex_generator_;
      _vertex_generator_ = nullptr;
      _owned_vertex_generator_ = false;
    }
    _vertex_generator_ = vertex_generator_ptr_;
    _owned_vertex_generator_ = true;
    return;
  }
  //_____________________________________________________________________________________
  VertexGeneratorInterface & PrimaryGeneratorAction::GetVertexGenerator() {
    return *_vertex_generator_;
  }
  //_____________________________________________________________________________________
  const VertexGeneratorInterface & PrimaryGeneratorAction::GetVertexGeneratorConst() const {
    return *_vertex_generator_;
  }
  //_____________________________________________________________________________________ 
  void PrimaryGeneratorAction::GeneratePrimaries(G4Event * event_) {
    if (IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::GeneratePrimaries: Entering..." << '\n';
    bxdecay0::event gendecay;
    _pimpl_->get_decay0().shoot(_pimpl_->get_prng(), gendecay);
    double event_time = 0.0 * CLHEP::second;
    if (gendecay.has_time()) {
      event_time = gendecay.get_time() * CLHEP::second;
    }
    double particle_time = 0.0 * CLHEP::second;
    const auto & particles = gendecay.get_particles();
    if (IsDebug()) std::cerr << "[debug] BxDecay0Generator::PrimaryGeneratorAction::GeneratePrimaries: Nb particles=" << particles.size() << '\n';

    G4ThreeVector vertex(0.0, 0.0, 0.0);
    if (HasVertexGenerator()) {
      if (not _vertex_generator_->HasNextVertex()) {
        G4RunManager::GetRunManager()->AbortRun();
        G4Exception("BxDecay0Generator::PrimaryGeneratorAction::GeneratePrimaries: ",
                    "InvalidArgument",
                    RunMustBeAborted,
                    "Vertex generator has no more vertex available! Abort run!");
      }
      _vertex_generator_->ShootVertex(vertex);  
    }

    for (const auto & particle : particles) {
      HitDataHandler* hit_data_handler = HitDataHandler::GetInstance();
      hit_data_handler->AddBxDecayParticle(&particle);

      dynamic_cast<RadiologicalGun*>(_particle_gun_)->ResetParticleData();
      if (particle.is_electron()) {
        _particle_gun_->SetParticleDefinition(G4Electron::ElectronDefinition());
      } else if (particle.is_positron()) {
        _particle_gun_->SetParticleDefinition(G4Positron::PositronDefinition());
      } else if (particle.is_gamma()) {
        _particle_gun_->SetParticleDefinition(G4Gamma::GammaDefinition());
      } else if (particle.is_alpha()) {
        _particle_gun_->SetParticleDefinition(G4Alpha::AlphaDefinition());
      } else {
        throw std::logic_error("bxdecay9_g4::PrimaryGeneratorAction::GeneratePrimaries: Unsupported particle type!");
      }
      
      if (particle.has_time()) {
        particle_time = particle.get_time() * CLHEP::second;
      } 
     
      particle_time += event_time;
      _particle_gun_->SetParticleTime(particle_time);
      
      G4ThreeVector momentum(particle.get_px() * CLHEP::MeV,
                             particle.get_py() * CLHEP::MeV,
                             particle.get_pz() * CLHEP::MeV);
      _particle_gun_->SetParticleMomentum(momentum);
      
      G4ThreeVector vertex(0.0, 0.0, 0.0);
      bulk_vertex_generator_->ShootVertex(vertex);
      _particle_gun_->SetParticlePosition(vertex);
      
      _particle_gun_->GeneratePrimaryVertex(event_);
    }
    if (IsTrace()) std::cerr << "[trace] BxDecay0Generator::PrimaryGeneratorAction::GeneratePrimaries: Exiting..." << '\n';
    return;
  }
} 