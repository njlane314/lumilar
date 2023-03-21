#ifndef MEDIUM_RESPONSE_HH
#define MEDIUM_RESPONSE_HH

#include "MaterialProperties.hh"
#include "Excitation.hh"
#include "Recombination.hh"

#include <random>
#include <string>
#include <tuple>

class MediumResponse {
 public:
    MediumResponse();
    ~MediumResponse();
    std::pair<double, double> create_response(EnergyDeposit* energy_deposit);
    
 private:
    std::pair<double, double> excite(EnergyDeposit* energy_deposit);
    std::pair<double, double> recombine(EnergyDeposit* energy_deposit, int num_ionisations, int num_excitations);
    double compute_recombination_factor(double linear_transfer, double electric_field);
    Properties* material_properties_;
};

#endif // MEDIUM_RESPONSE_HH
