#ifndef RELAXATION_HH
#define RELAXATION_HH

#include <iostream>
#include <chrono>

#include "G4SystemOfUnits.hh"

#include <CLHEP/Random/RandomEngine.h>
#include <CLHEP/Random/RandExponential.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Random/JamesRandom.h>
#include <CLHEP/Random/RandBinomial.h>

#include "LArNEST.h"

#include "EnergyDeposit.hh"
#include "MediumProperties.hh"

class Relaxation {
public:
    Relaxation();
    ~Relaxation();

    static double SampleEmissionTime(std::string particle_name);
};

#endif // RELAXATION_HH

