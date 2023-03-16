#ifndef RELAXATION_HH
#define RELAXATION_HH

#include "MaterialProperties.hh"
#include "OpticalPhoton.hh"
#include "TF1.h"
#include <iostream>

class Relaxation {
public:
    Relaxation();
    ~Relaxation();

    double sample_emission(double singlet_to_triplet);
    OpticalPhoton create_photon(double global_time, double singlet_to_triplet);

private:
    static double emission_probability(const double* x, const double* p);

    Properties* material_properties_;

    double singlet_lifetime_;
    double triplet_lifetime_;
    TF1* emission_probability_ = nullptr;
};

#endif
