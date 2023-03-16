#include "Relaxation.hh"

#include <chrono>

Relaxation::Relaxation()
: material_properties_(MaterialProperties::get_instance()->get_material_properties()) {
    singlet_lifetime_ = material_properties_->singlet_lifetime;
    triplet_lifetime_ = material_properties_->triplet_lifetime;

    emission_probability_ = new TF1("Emission Timing", emission_probability, 0., 3000., 4);
}

Relaxation::~Relaxation() {
    delete emission_probability_;
}

double Relaxation::sample_emission(double singlet_to_triplet) {
    double singlet_abundance = singlet_to_triplet / (1 + singlet_to_triplet);
    double triplet_abundance = 1 - singlet_abundance;

    emission_probability_->SetParameter(0, singlet_abundance);
    emission_probability_->SetParameter(1, triplet_abundance);
    emission_probability_->SetParameter(2, singlet_lifetime_);
    emission_probability_->SetParameter(3, triplet_lifetime_);
    emission_probability_->SetNpx(5000);

    auto start = std::chrono::high_resolution_clock::now();
    double result = emission_probability_->GetRandom();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken to sample emission probability: " << duration.count() << " microseconds" << std::endl;

    return result;
}

OpticalPhoton Relaxation::create_photon(double global_time, double singlet_to_triplet) {
    OpticalPhoton a_photon;
    a_photon.set_emission_time(global_time + sample_emission(singlet_to_triplet));

    return a_photon;
}

double Relaxation::emission_probability(const double* x, const double* p) {
    double t = *x;
    double singlet_abundance = p[0];
    double triplet_abundance = p[1];
    double singlet_lifetime = p[2];
    double triplet_lifetime = p[3];

    return singlet_abundance * std::exp(-t / singlet_lifetime) + triplet_abundance * std::exp(-t / triplet_lifetime);
}