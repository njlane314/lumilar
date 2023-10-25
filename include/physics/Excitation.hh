#ifndef EXCITATION_H
#define EXCITATION_H

#include <utility>

#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandPoisson.h"

#include "EnergyDeposit.hh"
#include "MediumProperties.hh"

class Excitation {
public:
    static std::pair<double, double> CreateExcitation(const EnergyDeposit* energy_deposit, const Properties* material_properties);
};

#endif // EXCITATION_H