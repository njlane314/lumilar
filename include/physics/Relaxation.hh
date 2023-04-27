#ifndef RELAXATION_HH
#define RELAXATION_HH

#include "MaterialProperties.hh"
#include "Recombination.hh"
#include "OpticalPhoton.hh"
#include "EnergyDeposit.hh"

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

    OpticalPhoton CreateOpticalPhoton(const EnergyDeposit* energy_deposit, double singlet_to_triplet);

 private:
    Properties* material_properties_;

    double singlet_lifetime_;
    double triplet_lifetime_;

    double SampleEmissionTime(double singlet_to_triplet);
    double QuenchedLifetime(double excited_rate);
};

#endif

