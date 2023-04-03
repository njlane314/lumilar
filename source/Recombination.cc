#include "Recombination.hh"

std::pair<double, double> Recombination::create_recombination(const EnergyDeposit* energy_deposit, const Properties* material_properties, const std::pair<double, double> intrinsic_response) {
    double recombination_factor = charge_recombination(energy_deposit->get_visible_energy(), material_properties->electric_field); 

    double num_ionisations = intrinsic_response.first;
    double num_excitations = intrinsic_response.second;

    int thermal_electrons = static_cast<int>(std::round(CLHEP::RandBinomial::shoot(num_ionisations, recombination_factor)));
    int optical_photons = num_excitations;

    if (thermal_electrons < 0) {
        thermal_electrons = 0;
    } else {
        optical_photons += num_ionisations - thermal_electrons;
    }

    return std::make_pair(thermal_electrons, optical_photons);
}

double Recombination::charge_recombination(double linear_transfer, double electric_field) {
    return birks_recombination(linear_transfer, electric_field) + escape_recombination(linear_transfer, electric_field);
}

double Recombination::birks_recombination(double linear_transfer, double electric_field) {
    double ARecomb = 0.800;
    double kRecomb = 0.0486;

    return ARecomb / (1. + linear_transfer * kRecomb / electric_field);
}

double Recombination::escape_recombination(double linear_transfer, double electric_field) {
    double larqlChi0A = 0.00338427;
    double larqlChi0B = -6.57037;
    double larqlChi0C = 1.88418;
    double larqlChi0D = 0.000129379;

    double escaping_fraction = larqlChi0A / (larqlChi0B + exp(larqlChi0C + larqlChi0D * linear_transfer));
    
    if (linear_transfer < 1) {
        linear_transfer = 1;
    }

    double larqlAlpha = 0.0372;
    double larqlBeta = 0.0124;

    double field_correction = std::exp(-electric_field  / (larqlAlpha * std::log(linear_transfer) + larqlBeta));

    return escaping_fraction * field_correction;
}