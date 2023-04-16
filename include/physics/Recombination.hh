#ifndef RECOMBINATION_HH
#define RECOMBINATION_HH

#include "MaterialProperties.hh"
#include "Excitation.hh"
#include "EnergyDeposit.hh"
#include <CLHEP/Random/RandBinomial.h>
#include <random> 

class Recombination {
 public: 
    static std::pair<double, double> ProcessRecombination(const EnergyDeposit* energy_deposit, const Properties* material_properties, const std::pair<double, double> intrinsic_response);
	static double charge_recombination(double linear_transfer, double electric_field);
    static double birks_recombination(double linear_transfer, double electric_field);
    static double escape_recombination(double linear_transfer, double electric_field);
};

#endif // RECOMBINATION_HH