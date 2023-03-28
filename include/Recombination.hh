#ifndef RECOMBINATION_HH
#define RECOMBINATION_HH

#include "MaterialProperties.hh"
#include "Excitation.hh"
#include <CLHEP/Random/RandBinomial.h>
#include <random> 

class Recombination {
 public: 
    static std::pair<double, double> create_recombination(EnergyDeposit* energy_deposit, Properties* material_properties, std::pair<double, double> intrinsic_response);

 private:
    static double charge_recombination(double linear_transfer, double electric_field);
};

#endif // RECOMBINATION_HH