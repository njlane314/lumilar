#include "Signal.hh"
#include "MaterialProperties.hh"

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
    } 
    //else if (material == "lArXe") {}
    
    return material_properties;
}
