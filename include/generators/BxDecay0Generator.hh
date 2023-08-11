#ifndef BXDECAY0G4_PRIMARY_GENERATOR_HH
#define BXDECAY0G4_PRIMARY_GENERATOR_HH

#include <memory>
#include <iostream>
#include <string>

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>

#include "BulkVertexGenerator.hh"
#include "HitDataHandler.hh"

class G4Event;
class G4ParticleGun;

#include <VertexGeneratorInterface.hh>

namespace BxDecay0Generator {

  class PrimaryGeneratorActionMessenger;
  
  class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
    struct ConfigurationInterface {
      void print(std::ostream & out_, const std::string & indent_ = "") const;
      void reset_base();
      void reset_mdl();
      void reset();
      bool is_valid_base() const;
      bool is_valid_mdl() const;
      bool is_valid() const;
      
      G4String  decay_category; 
      G4String  nuclide;        
      G4int     seed = 1;       
      G4int     dbd_mode = 0;   
      G4int     dbd_level = 0;  
      G4double  dbd_min_energy_MeV = -1.0;
      G4double  dbd_max_energy_MeV = -1.0;
      G4bool    debug = false;  

      G4bool    use_mdl = false;
      G4String  mdl_target_name = "";
      G4int     mdl_target_rank = -1;
      G4double  mdl_cone_longitude  = 0.0; // degree
      G4double  mdl_cone_colatitude = 0.0; // degree
      G4double  mdl_cone_aperture   = 0.0; // degree 
      G4double  mdl_cone_aperture2  = -1.0; // degree
      G4bool    mdl_error_on_missing_particle = false;
    };
    
    PrimaryGeneratorAction(int verbosity_ = 0);    
    PrimaryGeneratorAction(const ConfigurationInterface &, int verbosity_ = 0);    

    ~PrimaryGeneratorAction() override;

    const ConfigurationInterface & GetConfiguration() const;
    ConfigurationInterface & GrabConfiguration();

    void SetConfigHasChanged(bool);
    bool ConfigHasChanged() const;
    void SetConfiguration(const ConfigurationInterface &);
    void ApplyConfiguration();
    void DestroyConfiguration();
    void GeneratePrimaries(G4Event *) override;
    G4ParticleGun * GetParticleGun();
    bool HasVertexGenerator() const;
    void SetVerbosity(int level);
    int GetVerbosity() const;

    bool IsInfo() const;
    bool IsDebug() const;
    bool IsTrace() const;
    void Dump(std::ostream &) const;

    void SetVertexGenerator(VertexGeneratorInterface & vertex_generator_);
    void SetVertexGenerator(VertexGeneratorInterface * vertex_generator_ptr_);
    
    VertexGeneratorInterface & GetVertexGenerator();
    const VertexGeneratorInterface & GetVertexGeneratorConst() const;
    
  private:
    BulkVertexGenerator* bulk_vertex_generator_;
    G4ParticleGun * _particle_gun_ = nullptr; 
    VertexGeneratorInterface * _vertex_generator_ = nullptr;
    bool _owned_vertex_generator_ = false;
    PrimaryGeneratorActionMessenger * _messenger_ = nullptr; 
    ConfigurationInterface _config_; 
    bool _config_has_changed_ = false; 
    int _verbosity_ = 0; 
    
    struct pimpl_type;
    std::unique_ptr<pimpl_type> _pimpl_; 
  };
} 

#endif // BXDECAY0G4_PRIMARY_GENERATOR_HH