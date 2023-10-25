#ifndef SIGNAL_HH
#define SIGNAL_HH

#include <memory>
#include <Eigen/Dense>

#include "G4Step.hh"
#include "G4ThreeVector.hh"

#include "Scintillation.hh"
#include "Ionisation.hh"
#include "MediumResponse.hh"

#include "marley/Event.hh"
#include "marley/Generator.hh"
#include "marley/JSONConfig.hh"

class Signal {
public:
  Signal();
  ~Signal();
  
  static Signal* GetInstance();

  Scintillation* GetScintillation() const;
  Ionisation* GetIonisation() const;
  
  std::vector<EnergyDeposit>* GetHits() const;
  std::vector<double> GetEnergyDeposits() const;
  std::vector<double> GetLinearEnergyTransfers() const;
  std::vector<double> GetLengths() const;

  marley::Event& GetMarleyEvent();
  G4ThreeVector& GetMarleyInteractionVertex();

  void SetMarleyInteractionVertex(const G4ThreeVector& vertex);
  void SetMarleyEvent(const marley::Event& event);

  void AddEnergyDeposit(const EnergyDeposit* energy_deposit);

  void DeleteSignal();

private:
  EnergyDeposit* GetCurrentEnergyDeposit();

  static Signal* instance_;
  
  std::unique_ptr<Scintillation> scintillation_;
  std::unique_ptr<Ionisation> ionisation_;
  
  std::unique_ptr<EnergyDeposit> energy_deposit_;
  std::unique_ptr<std::vector<EnergyDeposit>> track_structure_;

  marley::Event marley_event_;
  G4ThreeVector marley_interaction_vertex_;
};

#endif // SIGNAL_HH
