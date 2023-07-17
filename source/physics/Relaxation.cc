#include "Relaxation.hh"
//_________________________________________________________________________________________
Relaxation::Relaxation() {}
//_________________________________________________________________________________________
Relaxation::~Relaxation() {}
//_________________________________________________________________________________________
double Relaxation::SampleEmissionTime() {
    //https://arxiv.org/pdf/2012.06527.pdf
    double singlet_abundance = 0.14;

    double emission_time;
    if (CLHEP::RandBinomial::shoot(1, singlet_abundance)) {
        double singlet_lifetime = 5. * ns;
        emission_time = CLHEP::RandExponential::shoot(singlet_lifetime);
    } else {
        double triplet_lifetime = 2100. * ns;
        double excited_rate = 1.3e-4;
        double quenched_rate = 2.3e-4;

        double quenched_lifetime = 1. / ((1. / triplet_lifetime) + excited_rate);
        double quenched_prob = 1 - log(1 + quenched_rate * quenched_lifetime) / (quenched_rate * triplet_lifetime);

        if (CLHEP::RandBinomial::shoot(1, quenched_prob) == 1) {
            return -1;
        } else {
            emission_time = CLHEP::RandExponential::shoot(quenched_lifetime);
        }
    } 

    return emission_time;
}