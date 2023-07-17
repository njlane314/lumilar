#ifndef SIGNAL_HH
#define SIGNAL_HH

#include <memory>
#include <Eigen/Dense>

#include "G4Step.hh"

#include "Scintillation.hh"
#include "Ionisation.hh"
#include "MediumResponse.hh"

class Signal {
public:
  Signal();
  ~Signal();
  
  static Signal* GetInstance();

  void AddEnergyDeposit(const EnergyDeposit* energy_deposit);
  
  Scintillation* GetScintillation() const;
  Ionisation* GetIonisation() const;
  
  std::vector<EnergyDeposit>* GetHits() const;
  std::vector<double> GetEnergyDeposits() const;
  std::vector<double> GetLinearEnergyTransfers() const;
  std::vector<double> GetLengths() const;

  void DeleteSignal();

private:
  EnergyDeposit* GetCurrentEnergyDeposit();

  static Signal* instance_;
  
  std::unique_ptr<Scintillation> scintillation_;
  std::unique_ptr<Ionisation> ionisation_;
  
  std::unique_ptr<EnergyDeposit> energy_deposit_;
  std::unique_ptr<std::vector<EnergyDeposit>> track_structure_;
  
  std::vector<double> delay_times_;
  double primary_energy_;
};

#endif // SIGNAL_HH
