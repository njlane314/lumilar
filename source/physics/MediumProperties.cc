#include "Signal.hh"
#include "MediumProperties.hh"

MediumProperties* MediumProperties::instance_ = nullptr;

MediumProperties::MediumProperties(std::string medium = "lAr") 
: medium_(medium) {
    if (!instance_) {
        instance_ = this;
        medium_properties_ = InitialiseProperties(medium_); 
    } 
}

MediumProperties::~MediumProperties() {}

MediumProperties* MediumProperties::GetInstance() {
    if (!instance_) {
        instance_ = new MediumProperties();
    }
    return instance_;
}

Properties* MediumProperties::GetMediumProperties() {
    return &medium_properties_;
}

Properties MediumProperties::InitialiseProperties(std::string medium = "lAr") {
    //medium = "lArXe";
    Properties medium_properties = {};

    medium_properties.loss_per_ionisation = 23.6 * eV;
    medium_properties.ionisation_threshold = 15.4 * eV;
    medium_properties.excitation_threshold = 12.7 * eV;
    medium_properties.thermal_loss = 4.3 * eV;

    medium_properties.fano_factor = 0.107;
    medium_properties.excitation_ratio = 0.21;

    medium_properties.singlet_lifetime = 5. * ns;
    medium_properties.triplet_lifetime = 2100. * ns;
    
    medium_properties.singlet_abundance = 0.14;
    medium_properties.triplet_abundance = 0.86;

    medium_properties.absorption_length = 20000; //mm -- 20 m

    medium_properties.excited_rate = 1.3e-4; // / nsec
    medium_properties.quenched_rate = 2.3e-4; // / nsec

    //https://pubs.aip.org/aip/jcp/article/91/3/1469/220871/Argon-krypton-and-xenon-excimer-luminescence-From
    medium_properties.argon_spectra_wavelength_mean = 127.;
    medium_properties.argon_spectra_wavelength_sigma = 7.4 / (2 * sqrt(2 * log(2)));

    medium_properties.xenon_spectra_wavelength_mean = 174.;
    medium_properties.xenon_spectra_wavelength_sigma = 8.9 / (2 * sqrt(2 * log(2)));

    if (medium == "lAr") {
        std::cout << "-- Setting medium properties for liquid argon." << std::endl;
        medium_properties.medium = "lAr";  
        medium_properties.is_doped = false;    
        medium_properties.quenching_enabled = true; 

        medium_properties.transfer_rate = 0.; // / ppm nsec
    } 
    else if (medium == "lArXe") {
        std::cout << "-- Setting medium properties for xenon-doped liquid argon." << std::endl;
        medium_properties.medium = "lArXe";
        medium_properties.is_doped = true;
        medium_properties.quenching_enabled = true;
       
        medium_properties.transfer_rate = 8.8e-5; // / ppm nsec
        medium_properties.doped_concentration = 20.; // ppm 

        medium_properties.xe_scint_profile = ParameteriseXeScintillationProfile(medium_properties.triplet_lifetime, medium_properties.excited_rate, medium_properties.transfer_rate, medium_properties.doped_concentration);
    }

    medium_properties.electric_field = 0.5; // kilovolt / cm
    
    return medium_properties;
}

std::vector<double> MediumProperties::ParameteriseXeScintillationProfile(double triplet_lifetime, double excited_rate, double transfer_rate, double doped_concentration) {
    std::cout << "-- Parameterising xenon scintillation profile." << std::endl;
    double quenched_lifetime = 1. / ((1. / triplet_lifetime) + excited_rate);
    double doped_lifetime = 1./(transfer_rate * doped_concentration);           
    double remaining_lifetime = 1./(1./doped_lifetime + 1./quenched_lifetime);

    double a = 1./doped_lifetime;
    double b = 1./remaining_lifetime;

    auto cdf_func = [a, b](double x) { /* Cumulative distribution function */
        return (1/b) * exp(-b*x) - (1/a) * exp(-a*x);
    };

    int x_min = 0;
    int x_max = 3000;
    int iterations = x_max - x_min;

    std::vector<double> cdf_vector(iterations);
    double max_cdf = 0;
    double cdf_0 = cdf_func(0);
    for (int i = 0; i < iterations; i++) {
        double cdf_val = cdf_func(i) - cdf_0;
        cdf_vector[i] = cdf_val;
        max_cdf = std::max(max_cdf, cdf_val);
    }
    for (int i = 0; i < iterations; i++) {
        cdf_vector[i] /= max_cdf;
    }

    return cdf_vector;
}

double MediumProperties::GetGroupVelocity(double wavelength) const {
    //https://arxiv.org/pdf/2002.09346.pdf
    const double c = 299792458;
    double delta_wl = 1e-10; 
    auto index_of_refraction = [wavelength](double wl) {
        double x = 0.334 + 0.1*wl*wl/(wl*wl - 106.6*106.6) + 0.008*wl*wl/(wl*wl - 908.3*908.3);
        return std::sqrt(1 + (3*x/(3-x)));
    };
    double dn_dlambda = (index_of_refraction(wavelength + delta_wl/2) - index_of_refraction(wavelength - delta_wl/2)) / delta_wl;
    return (c / (index_of_refraction(wavelength) - wavelength * dn_dlambda));
}