#ifndef SCINTILLATION_H
#define SCINTILLATION_H

#include <iostream>
#include <vector>

#include "OpticalPhoton.hh"
#include "Relaxation.hh"

struct PhotonRadiant {
    std::vector<double> position;
    std::vector<OpticalPhoton> photons;
};

class Scintillation {
 public:
    Scintillation();
    ~Scintillation();

    void add_radiant(int radiant_size, const std::vector<double>& position, double time, double singlet_to_triplet);
    std::vector<double> get_emission_times() const;
    std::vector<std::vector<double>> get_radiant_positions() const;
    std::vector<double> get_radiant_sizes() const;
    void print_scintillation() const;

 private:
    std::vector<PhotonRadiant> scintillation_;
    static Scintillation* instance_;
    Relaxation relaxation_generator_;
};

#endif // SCINTILLATION_H