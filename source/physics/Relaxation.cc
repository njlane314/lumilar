#include "Relaxation.hh"

Relaxation::Relaxation() : 
material_properties_(MaterialProperties::getInstance()->getMaterialProperties()), 
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

double Relaxation::SampleEmissionTime(double singlet_to_triplet) {
    double singlet_abundance = singlet_to_triplet / (1 + singlet_to_triplet);

    if (CLHEP::RandBinomial::shoot(1, singlet_abundance)) {
        return CLHEP::RandExponential::shoot(singlet_lifetime_);
    } else {
        return CLHEP::RandExponential::shoot(triplet_lifetime_);
    }
}

double Relaxation::QuenchedLifetime(double excited_rate) {
    return 1. / ((1. / triplet_lifetime_) + excited_rate);
}

OpticalPhoton Relaxation::CreateOpticalPhoton(const EnergyDeposit* energy_deposit, double singlet_to_triplet) {
    double emission_time = SampleEmissionTime(singlet_to_triplet);
    return (emission_time < 0) ? OpticalPhoton() : OpticalPhoton(energy_deposit->getTime() + emission_time);
}

