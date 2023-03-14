#include "Relaxation.hh"

Relaxation::Relaxation()
: material_properties_(MaterialProperties::get_instance()->get_material_properties()) {
    singlet_lifetime_ = material_properties_->singlet_lifetime;
    triplet_lifetime_ = material_properties_->triplet_lifetime;
    double singlet_to_triplet = material_properties_->singlet_to_triplet;

    singlet_abundance_ = singlet_to_triplet / (1 + singlet_to_triplet);
    triplet_abundance_ = 1 - singlet_abundance_;

    random_generator_ = std::mt19937(std::random_device()());
}

Relaxation::~Relaxation() {}

double Relaxation::sample_emission() {
    emission_distribution_ = std::uniform_real_distribution<double>(0, 1);
    double random_number = emission_distribution_(random_generator_);

    double t_initial = 0.;
    double t_final = 1000.;
    double t_step = 1.;

    double t = t_initial;
    double probability = 0.;
    while (probability < random_number && t < t_final) {
        probability += emission_probability(t) / (t_final - t_initial);
        t += t_step;
    }

    return t;
}

OpticalPhoton Relaxation::create_photon(double time) {
    OpticalPhoton a_photon;
    a_photon.set_emission_time(time + sample_emission());

    return a_photon;
}

double Relaxation::emission_probability(double t) {
    return singlet_abundance_ * std::exp(-t / singlet_lifetime_) + triplet_abundance_ * std::exp(-t / triplet_lifetime_);
}