#ifndef SCINTILLATION_H
#define SCINTILLATION_H

#include <iostream>
#include <vector>

#include "OpticalPhoton.hh"
#include "Relaxation.hh"
#include "Excitation.hh"
#include "EnergyDeposit.hh"

#include <Eigen/Dense>

struct PhotonRadiant {
    Eigen::Vector3d position;
    std::vector<OpticalPhoton> photons;
};

class Scintillation {
 public:
    Scintillation();
    ~Scintillation();

    void AddRadiant(const EnergyDeposit* energy_depsoit, int radiant_size, double singlet_to_triplet);
    std::vector<PhotonRadiant> getPhotonRadiants() const;
    int getTotalPhotonCount() const;
    std::vector<double> getEmissionTimes() const;
    std::vector<Eigen::Vector3d> getRadiantPositions() const;
    std::vector<int> getRadiantSizes() const;
    void PrintScintillation() const;

 private:
    static Scintillation* instance_;
    std::vector<PhotonRadiant> photon_radiants_;
    Relaxation relaxation_generator_;
};

#endif // SCINTILLATION_H