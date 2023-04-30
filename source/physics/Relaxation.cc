#include "Relaxation.hh"

Relaxation::Relaxation() {}

Relaxation::~Relaxation() {}

double Relaxation::SampleEmissionTime(double singlet_lifetime, double triplet_lifetime, double singlet_to_triplet, bool enable_quenching) {
    //https://arxiv.org/pdf/2012.06527.pdf
    double singlet_abundance = singlet_to_triplet / (1 + singlet_to_triplet);

    if (CLHEP::RandBinomial::shoot(1, singlet_abundance)) {
        return CLHEP::RandExponential::shoot(singlet_lifetime);
    } else {
        if (enable_quenching != true) {
            return CLHEP::RandExponential::shoot(triplet_lifetime);
        } else {
            double excited_rate = 1.3e-4;
            double quenched_rate = 2.3e-4;
            double quenched_lifetime = 1. / ((1. / triplet_lifetime) + excited_rate);
            double quenched_prob = log(1 + quenched_rate * quenched_lifetime) / (quenched_rate * triplet_lifetime);
            if (CLHEP::RandBinomial::shoot(1, quenched_prob)) {
                return -1.;
            } else {
                return CLHEP::RandExponential::shoot(quenched_lifetime);
            }
        }
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

    double emission_time = SampleEmissionTime(singlet_lifetime, triplet_lifetime, singlet_to_triplet, true);
    if (emission_time < 0) {
        return OpticalPhoton();
    }
    double wavelength = SampleWavelength(wavelength_mean, wavelength_fwhm);
    return (wavelength >= 0) ? OpticalPhoton(energy_deposit->GetTime() + emission_time, wavelength) : OpticalPhoton();
}

