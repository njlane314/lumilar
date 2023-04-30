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

    static OpticalPhoton CreateOpticalPhoton(const EnergyDeposit* energy_deposit);

 private:
    static double SampleEmissionTime(double singlet_lifetime, double triplet_lifetime, double singlet_to_triplet, bool enable_quenching);
	static double SampleWavelength(double wavelength_mean, double wavelength_fwhm);
};

#endif

