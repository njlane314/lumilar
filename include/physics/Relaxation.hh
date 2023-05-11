//____________________________________________________________________________
/*!

\class   physics::Relaxation

\brief   This class generates optical photons.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef RELAXATION_HH
#define RELAXATION_HH

#include <iostream>
#include <chrono>

#include <CLHEP/Random/RandomEngine.h>
#include <CLHEP/Random/RandExponential.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Random/JamesRandom.h>
#include <CLHEP/Random/RandBinomial.h>

#include <TF1.h>

#include "MediumProperties.hh"
#include "Recombination.hh"
#include "OpticalPhoton.hh"
#include "EnergyDeposit.hh"

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

