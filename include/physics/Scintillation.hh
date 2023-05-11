//____________________________________________________________________________
/*!

\class   physics::Scintillation

\brief   This class manages the scintillation signal.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef SCINTILLATION_HH
#define SCINTILLATION_HH

#include <iostream>
#include <vector>

#include <Eigen/Dense>

#include "Excitation.hh"
#include "Relaxation.hh"
#include "EnergyDeposit.hh"
#include "OpticalPhoton.hh"

struct PhotonRadiant {
    Eigen::Vector3d position;
    std::vector<OpticalPhoton> photons;
};

class Scintillation {
 public:
    Scintillation();
    ~Scintillation();

    void AddRadiant(const EnergyDeposit* energy_depsoit, int radiant_size);
    std::vector<PhotonRadiant> GetPhotonRadiants() const;
    int GetTotalPhotonCount() const;
    std::vector<double> GetEmissionTimes() const;
    std::vector<double> GetWavelengths() const;
    std::vector<Eigen::Vector3d> GetRadiantPositions() const;
    std::vector<int> GetRadiantSizes() const;
    void PrintScintillation() const;

 private:
    static Scintillation* instance_;
    std::vector<PhotonRadiant> photon_radiants_;
    Relaxation relaxation_generator_;
};

#endif // SCINTILLATION_HH