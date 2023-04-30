#include "Signal.hh"
#include "MaterialProperties.hh"
//change name to medium properties
MaterialProperties* MaterialProperties::instance_ = nullptr;

MaterialProperties::MaterialProperties(std::string material = "lAr") 
: material_(material) {
    if (!instance_) {
        instance_ = this;
        material_properties_ = InitialiseProperties(material_); 
    } 
}

MaterialProperties::~MaterialProperties() {}

MaterialProperties* MaterialProperties::GetInstance() {
    if (!instance_) {
        instance_ = new MaterialProperties();
    }
    return instance_;
}

Properties* MaterialProperties::GetMaterialProperties() {
    return &material_properties_;
}

Properties MaterialProperties::InitialiseProperties(std::string material = "lAr") {
    Properties material_properties = {};
    if (material == "lAr") {
        material_properties.material = "lAr";
        material_properties.loss_per_ionisation = 23.6 * eV;
        material_properties.ionisation_threshold = 15.4 * eV;
        material_properties.excitation_threshold = 12.7 * eV;
        material_properties.thermal_loss = 4.3 * eV;

        material_properties.fano_factor = 0.107;
        material_properties.excitation_ratio = 0.21;

        material_properties.singlet_lifetime = 5 * ns;
        material_properties.triplet_lifetime = 2100 * ns;

        material_properties.singlet_to_triplet_light = 0.33;
        material_properties.singlet_to_triplet_heavy = 3.;

        material_properties.electric_field = 0.5; // kilovolt / cm

        material_properties.excited_rate_light = 1.3 * 1e-4 * ns;
        material_properties.quenched_rate_light = 2.3 * 1e-4 * ns;

        material_properties.excited_rate_heavy = 0.0;
        material_properties.quenched_rate_heavy = 2.3 * 1e-3 * ns;

        material_properties.absorption_length = 20. * m;

        material_properties.wavelength_mean = 128.;
        material_properties.wavelength_fwhm = 6.;
    } 
    //else if (material == "lArXe") {}
    
    return material_properties;
}

double MaterialProperties::GetGroupVelocity(double wavelength) const {
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
