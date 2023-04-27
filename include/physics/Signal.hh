#ifndef SIGNAL_HH
#define SIGNAL_HH

#include "MaterialProperties.hh"
#include "MediumResponse.hh"

#include "Scintillation.hh"
#include "Ionisation.hh"

#include "G4Step.hh"
#include <memory>
#include <Eigen/Dense>

class Signal {
public:
  Signal();
  ~Signal();
  
  static Signal* getInstance();
  void ProcessResponse(const G4Step* step);
  Scintillation* getScintillation() const;
  Ionisation* getIonisation() const;
  std::vector<double> getVisibleDeposits() const;
  std::vector<double> getLinearTransfers() const;
  std::vector<double> getLengths() const;
  void DeleteSignal();
  void RecordDelayTime(const double delay_time);
  std::vector<double> getDelayTimes() const;
  void RecordPrimaryEnergy(const double primary_energy);
  double getPrimaryEnergy() const;

private:
  void CreateEnergyDeposit(const G4Step* step);
  EnergyDeposit* getCurrentEnergyDeposit();
  void AddEnergyDeposit(const EnergyDeposit* energy_deposit);

  static Signal* instance_;
  Properties* material_properties_;
  std::unique_ptr<Scintillation> scintillation_;
  std::unique_ptr<Ionisation> ionisation_;
  std::unique_ptr<EnergyDeposit> energy_deposit_;
  std::unique_ptr<std::vector<EnergyDeposit>> track_structure_;
  std::vector<double> delay_times_;
  double primary_energy_;
};

#endif // SIGNAL_HH
