#ifndef EXCITATION_HH
#define EXCITATION_HH

#include "MediumProperties.hh"
#include "EnergyDeposit.hh"

#include <CLHEP/Random/RandGauss.h>
#include <CLHEP/Random/RandPoisson.h>

#include <random>

class Excitation {
public: 
    static std::pair<double, double> CreateExctiation(const EnergyDeposit* energy_deposit, const Properties* material_properties);
};

#endif // EXCITATION_HH