#ifndef RELAXATION_HH
#define RELAXATION_HH

#include "MaterialProperties.hh"
#include "OpticalPhoton.hh"

#include <random>

class Relaxation {
 public:
    Relaxation();
    ~Relaxation();

    OpticalPhoton create_photon(double time);

 private:
    double singlet_lifetime_;
    double triplet_lifetime_;
    double singlet_abundance_;
    double triplet_abundance_;
    
    std::mt19937 random_generator_;
    std::uniform_real_distribution<double> emission_distribution_;
    Properties* material_properties_;

    double sample_emission();
    double emission_probability(double t);
};

#endif // RELAXATION_HH