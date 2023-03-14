#ifndef MEDIUM_RESPONSE_HH
#define MEDIUM_RESPONSE_HH

#include "MaterialProperties.hh"

#include <random>
#include <string>
#include <tuple>

struct EnergyDeposit {
    double visible;
    double linear_transfer;
    std::string particle_type;
    std::vector<double> position;
    double time;
};

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
    double thermal_electrons_;
    double optical_photons_;
};

#endif // MEDIUM_RESPONSE_HH
