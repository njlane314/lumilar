#include "Recombination.hh"

std::pair<double, double> Recombination::create_recombination(EnergyDeposit* energy_deposit, Properties* material_properties, std::pair<double, double> intrinsic_response) {
    double charge_recombination = charge_recombination(energy_deposit->linear_transfer, material_properties->electric_field); 

    double num_ionisations = intrinsic_response.first;
    double num_excitations = intrinsic_response.second;

    int thermal_electrons = static_cast<int>(std::round(CLHEP::RandBinomial::shoot(num_ionisations, charge_recombination)));
    int optical_photons = num_excitations;

    if (thermal_electrons < 0) {
        thermal_electrons = 0;
    } else {
        optical_photons += num_ionisations - thermal_electrons;
    }

    return std::make_pair(thermal_electrons, optical_photons);
}

double Recombination::charge_recombination(double linear_transfer, double electric_field) {
    double ARecomb = 0.800;
    //double BRecomb = ?;
    double kRecomb = 0.0486;

    return ARecomb / (1. + linear_transfer * kRecomb / electric_field); // + BRecomb
}