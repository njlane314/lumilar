#include "Relaxation.hh"

Relaxation::Relaxation() {}

Relaxation::~Relaxation() {}

std::pair<double, double> Relaxation::SampleEmissionTime(bool enable_quenching, bool is_doped) {
    //https://arxiv.org/pdf/2012.06527.pdf
    double singlet_abundance = MediumProperties::GetInstance()->GetMediumProperties()->singlet_abundance;

    double argon_spectra_wavelength_mean = MediumProperties::GetInstance()->GetMediumProperties()->argon_spectra_wavelength_mean;
    double argon_spectra_wavelength_sigma = MediumProperties::GetInstance()->GetMediumProperties()->argon_spectra_wavelength_sigma;

    if (is_doped == true) {
        enable_quenching = true;
    }
    double emission_time, wavelength;
    if (CLHEP::RandBinomial::shoot(1, singlet_abundance)) {
        double singlet_lifetime = MediumProperties::GetInstance()->GetMediumProperties()->singlet_lifetime;
        emission_time = CLHEP::RandExponential::shoot(singlet_lifetime);
        wavelength = SampleWavelength(argon_spectra_wavelength_mean, argon_spectra_wavelength_sigma);
    } else {
        double triplet_lifetime = MediumProperties::GetInstance()->GetMediumProperties()->triplet_lifetime;
        if (enable_quenching != true) {
            emission_time = CLHEP::RandExponential::shoot(triplet_lifetime);
            wavelength = SampleWavelength(argon_spectra_wavelength_mean, argon_spectra_wavelength_sigma);
        } else {
            double excited_rate = 1.3e-4;
            double quenched_rate = 2.3e-4;
            double quenched_lifetime = 1. / ((1. / triplet_lifetime) + excited_rate);
            if (is_doped == false) {
                double quenched_prob = log(1 + quenched_rate * quenched_lifetime) / (quenched_rate * triplet_lifetime);
                std::cout << "Quenched probability: " << quenched_prob << std::endl;
                if (CLHEP::RandBinomial::shoot(1, quenched_prob)) {
                    return std::make_pair(-1., -1.);
                } else {
                    emission_time = CLHEP::RandExponential::shoot(quenched_lifetime);
                    wavelength = SampleWavelength(argon_spectra_wavelength_mean, argon_spectra_wavelength_sigma);
                }
            } 
            else if (is_doped == true) {
                double xenon_spectra_wavelength_mean = MediumProperties::GetInstance()->GetMediumProperties()->xenon_spectra_wavelength_mean;
                double xenon_spectra_wavelength_sigma = MediumProperties::GetInstance()->GetMediumProperties()->xenon_spectra_wavelength_sigma;

                double transfer_rate = 8.8e-5; // /ppm nsec
                double doped_concentration = MediumProperties::GetInstance()->GetMediumProperties()->doped_concentration;
                double doped_lifetime = 1./(transfer_rate * doped_concentration);
                
                double remaining_lifetime = 1./(1./doped_lifetime + 1./quenched_lifetime);
                double triplet_prob = remaining_lifetime / triplet_lifetime;
                double shifted_prob = (transfer_rate * doped_concentration) / (transfer_rate * doped_concentration + 1./quenched_lifetime);
                
                double quenched_prob = 1 - triplet_prob - shifted_prob;
                std::cout << "Quenched probability: " << quenched_prob << std::endl;
                assert(quenched_prob > 0.);
                if (CLHEP::RandBinomial::shoot(1, quenched_prob)) {
                    return std::make_pair(-1., -1.);
                } else if (CLHEP::RandBinomial::shoot(1, triplet_prob/(1 - quenched_prob))) {
                    emission_time = CLHEP::RandExponential::shoot(remaining_lifetime);
                    wavelength = SampleWavelength(argon_spectra_wavelength_mean, argon_spectra_wavelength_sigma);
                } else {
                    double u = CLHEP::RandFlat::shoot();
                    emission_time = -std::log(1 - (u*(1 - std::exp(-(1/remaining_lifetime - 1/doped_lifetime)))))/(1/doped_lifetime);
                    wavelength = SampleWavelength(xenon_spectra_wavelength_mean, xenon_spectra_wavelength_sigma);
                }
            }
        }
    }

    return std::make_pair(emission_time, wavelength);
}

double Relaxation::SampleWavelength(double wavelength_mean, double wavelength_sigma) {
    //https://pubs.aip.org/aip/jcp/article/91/3/1469/220871/Argon-krypton-and-xenon-excimer-luminescence-From
    return CLHEP::RandGauss::shoot(wavelength_mean, wavelength_sigma);
}

OpticalPhoton Relaxation::CreateOpticalPhoton(const EnergyDeposit* energy_deposit) {
    auto medium_properties = MediumProperties::GetInstance()->GetMediumProperties();
    
    bool is_doped = medium_properties->is_doped;
    bool quenching_enabled = medium_properties->quenching_enabled;

    double emission_time, wavelength;
    std::tie(emission_time, wavelength) = SampleEmissionTime(quenching_enabled, is_doped);
    if (emission_time <= 0 || wavelength <= 0) {
        return OpticalPhoton();
    } else {
        return OpticalPhoton(energy_deposit->GetTime() + emission_time, wavelength);
    }
}