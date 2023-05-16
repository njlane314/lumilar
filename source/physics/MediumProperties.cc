#include "Signal.hh"
#include "MediumProperties.hh"
//_________________________________________________________________________________________
MediumProperties* MediumProperties::instance_ = nullptr;
//_________________________________________________________________________________________
MediumProperties::MediumProperties(double xenon_concentration = 0.0, double nitrogen_concentration = 2.) : xenon_concentration_(xenon_concentration), nitrogen_concentration_(nitrogen_concentration) {
    if (!instance_) {
        instance_ = this;
        medium_properties_ = InitialiseProperties(); 
    } 
}
//_________________________________________________________________________________________
MediumProperties::~MediumProperties() {}
//_________________________________________________________________________________________
MediumProperties* MediumProperties::GetInstance() {
    if (!instance_) {
        instance_ = new MediumProperties();
    }
    return instance_;
}
//_________________________________________________________________________________________
Properties* MediumProperties::GetMediumProperties() {
    return &medium_properties_;
}
//_________________________________________________________________________________________
Properties MediumProperties::InitialiseProperties() {
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

    medium_properties.excited_rate = 1.3e-4; // / nsec
    medium_properties.quenched_rate = 2.3e-4; // / nsec

    medium_properties.nitrogen_concentration = nitrogen_concentration_; // ppm

    //https://pubs.aip.org/aip/jcp/article/91/3/1469/220871/Argon-krypton-and-xenon-excimer-luminescence-From
    medium_properties.argon_spectra_wavelength_mean = 127.;
    medium_properties.argon_spectra_wavelength_sigma = 7.4 / (2 * sqrt(2 * log(2)));

    medium_properties.xenon_concentration = xenon_concentration_; // ppm
    std::cout << "-- Xenon concentration: " << medium_properties.xenon_concentration << " ppm." << std::endl;

    medium_properties.xenon_spectra_wavelength_mean = 174.;
    medium_properties.xenon_spectra_wavelength_sigma = 8.9 / (2 * sqrt(2 * log(2)));

    medium_properties.transfer_rate = 8.8e-5; // / ppm nsec

    if (medium_properties.xenon_concentration > 0) {
        medium_properties.xe_scint_profile = ParameteriseXeScintillationProfile(medium_properties.triplet_lifetime, medium_properties.excited_rate, medium_properties.transfer_rate, medium_properties.xenon_concentration);
    } else {
        medium_properties.xe_scint_profile = {-1.};
    }

    medium_properties.electric_field = 0.5; // kilovolt / cm
    
    return medium_properties;
}
//_________________________________________________________________________________________
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
//_________________________________________________________________________________________
double MediumProperties::GetGroupVelocity(double wl) const {
    //https://arxiv.org/pdf/2002.09346.pdf
    const double c = 299792458;
    double delta_wl = 1e-10; 
    
    double dn_dlambda = (GetRefractiveIndex(wl + delta_wl/2) - GetRefractiveIndex(wl - delta_wl/2)) / delta_wl;
    return (c / (GetRefractiveIndex(wl) - wl * dn_dlambda));
}
//_________________________________________________________________________________________
double MediumProperties::GetRefractiveIndex(double wl) const {
    double x = 0.334 + 0.1*wl*wl/(wl*wl - 106.6*106.6) + 0.008*wl*wl/(wl*wl - 908.3*908.3);
    return std::sqrt(1 + (3*x/(3-x)));
}
//_________________________________________________________________________________________
double MediumProperties::GetRayleighScatteringLength(double wl) const {
    double n = GetRefractiveIndex(wl);
    wl *= 1e-9; // m
    double T = 90; // K
    double iso_compress = 2.21e-4 * 1e-5; // m2 / kg
    double boltz_k = 1.380649e-23; // m2 kg s-2 K-1
    double inverse_rl = (16*3.14159*3.14159*3.14159/ (6*wl*wl*wl*wl)) * (boltz_k*T*iso_compress*pow((n*n - 1) * (n*n + 2) / 3, 2));
    return 1000. / inverse_rl; // mm
}
//_________________________________________________________________________________________
double MediumProperties::GetAbsorptionLength() const {
    // independent of wavelength
    //https://arxiv.org/abs/1306.4605
    double p = 1.51e-4; // cm-1 ppm-1
    return - 1000. / (100. * log(1 - p*medium_properties_.nitrogen_concentration)); // mm
}