#ifndef SIGNAL_HH
#define SIGNAL_HH

#include "MaterialProperties.hh"
#include "MediumResponse.hh"

#include "Scintillation.hh"
#include "Ionisation.hh"

#include "G4Step.hh"
#include <memory>

class Signal {
 public:
  Signal();
  ~Signal();
  
  static Signal* get_instance();
  void process_response(const G4Step* step);
  Scintillation* get_scintillation();
  Ionisation* get_ionisation();
  std::vector<double> get_visible_deposits() const;
  std::vector<double> get_linear_transfers() const;
  std::vector<double> get_lengths() const;
  void delete_signal();

 private:
  void create_energy_deposit(const G4Step* step);
  EnergyDeposit* get_current_energy_deposit();
  void add_energy_deposit(const EnergyDeposit* energy_deposit);

  static Signal* instance_;
  Properties* material_properties_;
  std::unique_ptr<Scintillation> scintillation_;
  std::unique_ptr<Ionisation> ionisation_;
  std::unique_ptr<EnergyDeposit> energy_deposit_;
  std::unique_ptr<std::vector<EnergyDeposit>> track_structure_;
};

#endif // SIGNAL_HH
