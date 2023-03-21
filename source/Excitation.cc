#include "Excitation.hh"

std::pair<double, double> Excitation::create_exctiation(const EnergyDeposit* energy_deposit, const Properties* material_properties) {
    std::random_device rd;
    std::mt19937 eng(rd());
    
    double expected_ionisations = energy_deposit->visible / material_properties->loss_per_ionisation;

    int num_ionisations;
    if (expected_ionisations > 10) {
        double resolution = material_properties->fano_factor * std::sqrt(expected_ionisations);
        std::normal_distribution<double> dist(expected_ionisations, resolution);
        num_ionisations = static_cast<int>(std::round(dist(eng)));
    } else {
        std::poisson_distribution<int> dist(expected_ionisations);
        num_ionisations = dist(eng);
    }

    double excitation_energy = energy_deposit->visible - num_ionisations * (material_properties->ionisation_threshold + material_properties->thermal_loss);
    int num_excitations = excitation_energy / material_properties->excitation_threshold;

    return std::make_pair(num_ionisations, num_excitations);
}