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
  void delete_signal();

 private:
  EnergyDeposit* create_energy_deposit(const G4Step* step);

  static Signal* instance_;
  Properties* material_properties_;
  std::unique_ptr<Scintillation> scintillation_;
  std::unique_ptr<Ionisation> ionisation_;
};

#endif // SIGNAL_HH
