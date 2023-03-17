#ifndef SIGNAL_HH
#define SIGNAL_HH

#include "MaterialProperties.hh"
#include "MediumResponse.hh"
#include "Scintillation.hh"

#include "G4Step.hh"

class Signal {
 public:
  Signal();
  ~Signal();
  
  static Signal* get_instance();
  void process_response(const G4Step* step);
  Scintillation* get_scintillation();

 private:
  EnergyDeposit* create_energy_deposit(const G4Step* step);

  static Signal* instance_;
  Properties* material_properties_;
  Scintillation* scintillation_;
};

#endif // SIGNAL_HH
