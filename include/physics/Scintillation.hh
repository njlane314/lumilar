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

    void add_radiant(const EnergyDeposit* energy_depsoit, int radiant_size, double singlet_to_triplet);
    std::vector<PhotonRadiant> get_photon_radiants() const;
    std::vector<double> get_emission_times() const;
    std::vector<Eigen::Vector3d> get_radiant_positions() const;
    std::vector<double> get_radiant_sizes() const;
    void print_scintillation() const;

 private:
    std::vector<PhotonRadiant> scintillation_;
    static Scintillation* instance_;
    Relaxation relaxation_generator_;
};

#endif // SCINTILLATION_H