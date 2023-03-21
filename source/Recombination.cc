#include "Recombination.hh"

std::pair<double, double> Recombination::create_recombination(EnergyDeposit* energy_deposit, Properties* material_properties, std::pair<double, double> intrinsic_response) {
    double recombination_factor = compute_recombination_factor(energy_deposit->linear_transfer, material_properties->electric_field); 

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

// charge_recombination
double Recombination::compute_recombination_factor(double linear_transfer, double electric_field) {
    auto BirksRecomb = [&]() -> double {
        double ARecomb = 0.800;
        double kRecomb = 0.0486;

        return ARecomb / (1. + linear_transfer * kRecomb / electric_field);
    };

    auto EscapeRecomb = [&]() -> double {
        auto EscapingFraction = [linear_transfer]() -> double {
            double larqlChi0A = 0.00338427;
            double larqlChi0B = -6.57037;
            double larqlChi0C = 1.88418;
            double larqlChi0D = 0.000129379;

            return larqlChi0A / (larqlChi0B + exp(larqlChi0C + larqlChi0D * linear_transfer));
        };

        auto FieldCorrection = [&]() -> double {
            if (linear_transfer < 1) {
                return 0;
            }
            else {
                double larqlAlpha = 0.0372;
                double larqlBeta = 0.0124;

                return std::exp(-electric_field  / (larqlAlpha * std::log(linear_transfer) + larqlBeta));
            }
        };

        return EscapingFraction() * FieldCorrection();
    };

    return BirksRecomb() + EscapeRecomb();
}