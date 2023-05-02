#ifndef RELAXATION_HH
#define RELAXATION_HH

#include "MediumProperties.hh"
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

#include <TF1.h>

class Relaxation {
public:
    Relaxation();
    ~Relaxation();

    static OpticalPhoton CreateOpticalPhoton(const EnergyDeposit* energy_deposit);

private:
    static std::pair<double, double> SampleEmissionTime(bool enable_quenching, bool is_doped);
	static double SampleWavelength(double wavelength_mean, double wavelength_sigma);
    static double SampleXeScintillationProfile();
};

#endif

