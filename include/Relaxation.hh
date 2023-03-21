#ifndef RELAXATION_HH
#define RELAXATION_HH

#include "MaterialProperties.hh"
#include "OpticalPhoton.hh"

#include <CLHEP/Random/RandomEngine.h>
#include <CLHEP/Random/RandExponential.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Random/JamesRandom.h>
#include <CLHEP/Random/RandBinomial.h>

#include <iostream>
#include <chrono>

class Relaxation {
public:
    Relaxation();
    ~Relaxation();

    double sample_emission(double singlet_to_triplet);
    OpticalPhoton create_photon(double global_time, double singlet_to_triplet);

private:
    Properties* material_properties_;

    double singlet_lifetime_;
    double triplet_lifetime_;
};

#endif

