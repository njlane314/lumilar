//____________________________________________________________________________
/*!

\class   physics::Recombination

\brief   This class contains methods for calculating charge recombination.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef RECOMBINATION_HH
#define RECOMBINATION_HH

#include <random> 

#include <CLHEP/Random/RandBinomial.h>

#include "Excitation.hh"
#include "EnergyDeposit.hh"
#include "MediumProperties.hh"

class Recombination {
 public: 
    static std::pair<double, double> ProcessRecombination(const EnergyDeposit* energy_deposit, const Properties* material_properties, const std::pair<double, double> intrinsic_response);
	static double ChargeRecombination(double linear_transfer, double electric_field);
    static double BirksRecombination(double linear_transfer, double electric_field);
    static double EscapeRecombination(double linear_transfer, double electric_field);
};

#endif // RECOMBINATION_HH