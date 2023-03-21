#ifndef EXCITATION_HH
#define EXCITATION_HH

#include "MaterialProperties.hh"
#include <CLHEP/Random/RandGauss.h>
#include <CLHEP/Random/RandPoisson.h>

struct EnergyDeposit {
    double visible;
    double linear_transfer;
    std::string particle_type;
    std::vector<double> position;
    double time;
};

class Excitation {
 public: 
    static std::pair<double, double> create_exctiation(const EnergyDeposit* energy_deposit, const Properties* material_properties);
};

#endif // EXCITATION_HH