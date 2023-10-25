#include "Relaxation.hh"
//_________________________________________________________________________________________
Relaxation::Relaxation() {}
//_________________________________________________________________________________________
Relaxation::~Relaxation() {}
//_________________________________________________________________________________________
double Relaxation::SampleEmissionTime(const std::string particle_name) {
    Properties* medium_properties = MediumProperties::GetInstance()->GetMediumProperties();
    double singlet_lifetime = medium_properties->singlet_lifetime;
    double triplet_lifetime = medium_properties->triplet_lifetime;
    double singlet_abundance;

    if (particle_name == "e+" || particle_name == "e-") {
        singlet_abundance = medium_properties->electron_singlet_abundance;
    } else if (particle_name == "mu+" || particle_name == "mu-") {
        singlet_abundance = medium_properties->muon_singlet_abundance;
    } else if (particle_name == "pi+" || particle_name == "pi-") {
        singlet_abundance = medium_properties->pion_singlet_abundance;
    } else if (particle_name == "kaon+" || particle_name == "kaon-") { 
        singlet_abundance = medium_properties->kaon_singlet_abundance;
    } else if (particle_name == "proton" || particle_name == "anti_proton") {
        singlet_abundance = medium_properties->proton_singlet_abundance;
    } else if (particle_name == "alpha") {
        singlet_abundance = medium_properties->alpha_singlet_abundance;
    } else {
        return -1.;
    }

    double emission_time;
    if (CLHEP::RandBinomial::shoot(1, singlet_abundance)) {
        emission_time = CLHEP::RandExponential::shoot(singlet_lifetime);
    } else {
        emission_time = CLHEP::RandExponential::shoot(triplet_lifetime);
    }

    return emission_time;
}