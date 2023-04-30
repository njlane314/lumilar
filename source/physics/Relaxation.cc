#include "Relaxation.hh"

Relaxation::Relaxation() {}

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
}

double Relaxation::QuenchedLifetime(double excited_rate) {
    return 1. / ((1. / triplet_lifetime_) + excited_rate);
}*/

double Relaxation::SampleEmissionTime(double singlet_lifetime, double triplet_lifetime, double singlet_to_triplet) {
    double singlet_abundance = singlet_to_triplet / (1 + singlet_to_triplet);

    if (CLHEP::RandBinomial::shoot(1, singlet_abundance)) {
        return CLHEP::RandExponential::shoot(singlet_lifetime);
    } else {
        return CLHEP::RandExponential::shoot(triplet_lifetime);
    }
}

double Relaxation::SampleWavelength(double wavelength_mean, double wavelength_fwhm) {
    //https://pubs.aip.org/aip/jcp/article/91/3/1469/220871/Argon-krypton-and-xenon-excimer-luminescence-From
    double wavelength_sigma = wavelength_fwhm / (2 * sqrt(2 * log(2)));
    return CLHEP::RandGauss::shoot(wavelength_mean, wavelength_sigma);
}

OpticalPhoton Relaxation::CreateOpticalPhoton(const EnergyDeposit* energy_deposit) {
    auto material_properties = MaterialProperties::GetInstance()->GetMaterialProperties();
    double singlet_lifetime, triplet_lifetime, singlet_to_triplet, wavelength_mean, wavelength_fwhm;
    
    if (material_properties->material == "lAr") {
        singlet_lifetime = material_properties->singlet_lifetime;
        triplet_lifetime = material_properties->triplet_lifetime;
        singlet_to_triplet = material_properties->singlet_to_triplet_light;
        
        wavelength_mean = material_properties->wavelength_mean;
        wavelength_fwhm = material_properties->wavelength_fwhm;
    } 
    else {
        throw std::runtime_error("-- Medium not supported for optical photon creation. Aborting.");
    }

    double emission_time = SampleEmissionTime(singlet_lifetime, triplet_lifetime, singlet_to_triplet);
    double wavelength = SampleWavelength(wavelength_mean, wavelength_fwhm);

    if (emission_time < 0 || wavelength < 0) {
        return OpticalPhoton();
    } else {
        return OpticalPhoton(energy_deposit->GetTime() + emission_time, wavelength);
    }
}

