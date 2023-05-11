//____________________________________________________________________________
/*!

\class   physics::Excitation

\brief   This class contains methods for calculating the electronic excitation
         of the detector medium.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef EXCITATION_HH
#define EXCITATION_HH

#include <random>

#include <CLHEP/Random/RandGauss.h>
#include <CLHEP/Random/RandPoisson.h>

#include "EnergyDeposit.hh"
#include "MediumProperties.hh"

class Excitation {
public: 
    static std::pair<double, double> CreateExctiation(const EnergyDeposit* energy_deposit, const Properties* material_properties);
};

#endif // EXCITATION_HH