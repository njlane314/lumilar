#include "Excitation.hh"
//_________________________________________________________________________________________
std::pair<double, double> Excitation::CreateExcitation(const EnergyDeposit* energy_deposit, const Properties* material_properties) {    
    double expected_ionisations = energy_deposit->GetVisibleEnergy() / material_properties->loss_per_ionisation;

    int num_ionisations = 0;
    int num_excitations = 0;
    if (expected_ionisations > 10) {
        double resolution =  std::sqrt(material_properties->fano_factor * expected_ionisations);
        num_ionisations = static_cast<int>(std::round(CLHEP::RandGauss::shoot(expected_ionisations, resolution)));
    } else {
        num_ionisations = static_cast<int>(std::round(CLHEP::RandPoisson::shoot(expected_ionisations)));
    }

    double excitation_energy = energy_deposit->GetVisibleEnergy() - num_ionisations * (material_properties->ionisation_threshold + material_properties->thermal_loss);
    if (excitation_energy > 0) {
        num_excitations = excitation_energy / material_properties->excitation_threshold;
    }

    return std::make_pair(num_ionisations, num_excitations);
}