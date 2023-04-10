#include "Recombination.hh"

std::pair<double, double> Recombination::ProcessRecombination(const EnergyDeposit* energy_deposit, const Properties* material_properties, const std::pair<double, double> intrinsic_response) {
    double recombination_factor = BirksRecombination(energy_deposit->get_visible_energy(), material_properties->electric_field); 

    double num_ionisations = intrinsic_response.first;
    double num_excitations = intrinsic_response.second;

    int thermal_electrons = static_cast<int>(std::round(CLHEP::RandBinomial::shoot(num_ionisations, recombination_factor)));
    int optical_photons = num_excitations;

    if (thermal_electrons < 0) {
        thermal_electrons = 0;
    } else {
        optical_photons += num_ionisations - thermal_electrons;
    }

    //std::cout << "Recombination: " << thermal_electrons << " thermal electrons, " << optical_photons << " optical photons" << std::endl;

    return std::make_pair(thermal_electrons, optical_photons);
}

double Recombination::BirksRecombination(double linear_transfer, double electric_field) {
    double ARecomb;
    double BRecomb;
    double CRecomb;

    MaterialProperties::get_instance()->get_recombination_parameters(ARecomb, BRecomb, CRecomb);

    return (ARecomb * linear_transfer) / (1. + BRecomb * linear_transfer) + CRecomb;
}