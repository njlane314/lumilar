#ifndef SCINTILLATION_HH
#define SCINTILLATION_HH

#include <iostream>
#include <vector>

#include <Eigen/Dense>

#include "Relaxation.hh"
#include "EnergyDeposit.hh"
#include "PhotonRadiant.hh"

class Scintillation {
 public:
    Scintillation();
    ~Scintillation();

    void CreateRadiant(const EnergyDeposit* energy_depsoit, int optical_photon_count);
    
    std::vector<PhotonRadiant> GetPhotonRadiants() const;
    int GetTotalPhotonCount() const;
    std::vector<Eigen::Vector3d> GetRadiantPositions() const;
    std::vector<int> GetRadiantCounts() const;

 private:
    static Scintillation* instance_;
    std::vector<PhotonRadiant> photon_radiant_vector_;
};

#endif // SCINTILLATION_HH
