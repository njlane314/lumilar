#include "MediumResponse.hh"

MediumResponse::MediumResponse() 
: material_properties_(MaterialProperties::get_instance()->get_material_properties()) {}

MediumResponse::~MediumResponse() {}

std::pair<double, double> MediumResponse::create_response(EnergyDeposit* energy_deposit) {
    return excite(energy_deposit);
}

std::pair<double, double> MediumResponse::excite(EnergyDeposit* energy_deposit) {
    std::random_device rd;
    std::mt19937 eng(rd());
    
    double expected_ionisations = energy_deposit->visible / material_properties_->loss_per_ionisation;

    int num_ionisations;
    if (expected_ionisations > 10) {
        double resolution = material_properties_->fano_factor * std::sqrt(expected_ionisations);
        std::normal_distribution<double> dist(expected_ionisations, resolution);
        num_ionisations = static_cast<int>(std::round(dist(eng)));
    } else {
        std::poisson_distribution<int> dist(expected_ionisations);
        num_ionisations = dist(eng);
    }

    double excitation_energy = energy_deposit->visible - num_ionisations * (material_properties_->ionisation_threshold + material_properties_->thermal_loss);
    int num_excitations = excitation_energy / material_properties_->excitation_threshold;

    int num_thermal_electrons, num_optical_photons;
    std::tie(num_thermal_electrons, num_optical_photons) = recombine(energy_deposit, num_ionisations, num_excitations);
    
    return std::make_pair(num_thermal_electrons, num_optical_photons);
}

std::pair<double, double> MediumResponse::recombine(EnergyDeposit* energy_deposit, int num_ionisations, int num_excitations) {
    std::random_device rd;
    std::mt19937 eng(rd());
    double recombination_factor = compute_recombination_factor(energy_deposit->linear_transfer, material_properties_->electric_field); 

    std::normal_distribution<double> dist(num_ionisations * recombination_factor, num_ionisations * recombination_factor * (1 - recombination_factor));
    
    thermal_electrons_ = dist(eng);
    optical_photons_ = num_excitations + (num_ionisations - thermal_electrons_);

    return std::make_pair(thermal_electrons_, optical_photons_);
}

double MediumResponse::compute_recombination_factor(double linear_transfer, double electric_field) {
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