#include "Relaxation.hh"

Relaxation::Relaxation() : 
material_properties_(MaterialProperties::get_instance()->get_material_properties()), 
singlet_lifetime_(material_properties_->singlet_lifetime),
triplet_lifetime_(material_properties_->triplet_lifetime) {}

Relaxation::~Relaxation() {}

/*double Relaxation::sample_emission(double linear_transfer, double singlet_to_triplet) {
    double singlet_abundance = singlet_to_triplet / (1 + singlet_to_triplet);

    if (CLHEP::RandBinomial::shoot(1, singlet_abundance)) {
        return CLHEP::RandExponential::shoot(singlet_lifetime_);
    } else {
        if (material_properties_->material == "lAr") {
            double ABirks = 0.800;
            double excited_rate = material_properties_->excited_rate_light * (1 + (Recombination::birks_recombination(linear_transfer, material_properties_->electric_field) / Recombination::escape_recombination(linear_transfer, material_properties_->electric_field)));
            double quenched_rate = material_properties_->quenched_rate_light * (1 - (Recombination::birks_recombination(linear_transfer, material_properties_->electric_field) / (ABirks + material_properties_->excitation_ratio)));

            double quenched_lifetime_ = quenched_lifetime(excited_rate);

            double quenched_prob = log(1 + quenched_rate * quenched_lifetime_) / (quenched_rate * triplet_lifetime_);

            if (CLHEP::RandBinomial::shoot(1, quenched_prob)) { 
                return -1;
            } else {
                return CLHEP::RandExponential::shoot(quenched_lifetime_);
            }
        }
        else if (material_properties_->material == "lArXe") {
            return -1;
        }
    }
}*/

double Relaxation::sample_emission(double singlet_to_triplet) {
    double singlet_abundance = singlet_to_triplet / (1 + singlet_to_triplet);

    if (CLHEP::RandBinomial::shoot(1, singlet_abundance)) {
        return CLHEP::RandExponential::shoot(singlet_lifetime_);
    } else {
        return CLHEP::RandExponential::shoot(triplet_lifetime_);
    }
}

double Relaxation::quenched_lifetime(double excited_rate) {
    return 1. / ((1. / triplet_lifetime_) + excited_rate);
}

OpticalPhoton Relaxation::create_photon(const EnergyDeposit* energy_deposit, double singlet_to_triplet) {
    //double emission_time = sample_emission(energy_deposit->get_linear_transfer(), singlet_to_triplet);
    double emission_time = sample_emission(singlet_to_triplet);
    return (emission_time < 0) ? OpticalPhoton() : OpticalPhoton(energy_deposit->get_time() + emission_time);
}
