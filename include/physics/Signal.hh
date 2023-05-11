//____________________________________________________________________________
/*!

\class   physics::Signal

\brief   This class manages the signal response.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef SIGNAL_HH
#define SIGNAL_HH

#include <memory>
#include <Eigen/Dense>

#include "G4Step.hh"

#include "Scintillation.hh"
#include "Ionisation.hh"
#include "MediumResponse.hh"
#include "MediumProperties.hh"

class Signal {
public:
  Signal();
  ~Signal();
  
  static Signal* GetInstance();
  void ProcessResponse(const G4Step* step);
  Scintillation* GetScintillation() const;
  Ionisation* GetIonisation() const;
  std::vector<double> GetVisibleDeposits() const;
  std::vector<double> GetLinearTransfers() const;
  std::vector<double> GetLengths() const;
  void DeleteSignal();
  void RecordDelayTime(const double delay_time);
  std::vector<double> GetDelayTimes() const;
  void RecordPrimaryEnergy(const double primary_energy);
  double GetPrimaryEnergy() const;
  void AddEnergyDeposit(const EnergyDeposit* energy_deposit);

private:
  void CreateEnergyDeposit(const G4Step* step);
  EnergyDeposit* GetCurrentEnergyDeposit();

  static Signal* instance_;
  Properties* medium_properties_;
  std::unique_ptr<Scintillation> scintillation_;
  std::unique_ptr<Ionisation> ionisation_;
  std::unique_ptr<EnergyDeposit> energy_deposit_;
  std::unique_ptr<std::vector<EnergyDeposit>> track_structure_;
  std::vector<double> delay_times_;
  double primary_energy_;
};

#endif // SIGNAL_HH
