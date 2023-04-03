#include "Relaxation.hh"

Relaxation::Relaxation() : 
material_properties_(MaterialProperties::get_instance()->get_material_properties()), 
singlet_lifetime_(material_properties_->singlet_lifetime),
triplet_lifetime_(material_properties_->triplet_lifetime) {}

Relaxation::~Relaxation() {}

double Relaxation::sample_emission(double singlet_to_triplet) {
    double singlet_abundance = singlet_to_triplet / (1 + singlet_to_triplet);

    if (CLHEP::RandBinomial::shoot(1, singlet_abundance)) {
        return CLHEP::RandExponential::shoot(singlet_lifetime_);
    } else {
        double quenched_lifetime = 1. / ((1. / triplet_lifetime_) + material_properties_->excited_rate_light);
        double quenched_prob = log(1 + material_properties_->quenched_rate_light * quenched_lifetime) / (material_properties_->quenched_rate_light * triplet_lifetime_);
        if (CLHEP::RandBinomial::shoot(1, quenched_prob)) {
            return -1;
        } else {
            return CLHEP::RandExponential::shoot(triplet_lifetime_);
        }
    }
}

OpticalPhoton Relaxation::create_photon(double global_time, double singlet_to_triplet) {
    double emission_time = sample_emission(singlet_to_triplet);
    return (emission_time < 0) ? OpticalPhoton() : OpticalPhoton(global_time + emission_time);
}