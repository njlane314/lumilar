#include "Relaxation.hh"

Relaxation::Relaxation()
: material_properties_(MaterialProperties::get_instance()->get_material_properties()) {
    singlet_lifetime_ = material_properties_->singlet_lifetime;
    triplet_lifetime_ = material_properties_->triplet_lifetime;
}

Relaxation::~Relaxation() {}

double Relaxation::sample_emission(double singlet_to_triplet) {
    double singlet_abundance = singlet_to_triplet / (1 + singlet_to_triplet);
    double triplet_abundance = 1 - singlet_abundance;

    if (CLHEP::RandBinomial::shoot(1, singlet_abundance) == 1) {
        return CLHEP::RandExponential::shoot(singlet_lifetime_ * log(2.0));
    } else {
        return CLHEP::RandExponential::shoot(triplet_lifetime_ * log(2.0));
    }
}

OpticalPhoton Relaxation::create_photon(double global_time, double singlet_to_triplet) {
    OpticalPhoton a_photon;
    a_photon.set_emission_time(global_time + sample_emission(singlet_to_triplet));

    return a_photon;
}