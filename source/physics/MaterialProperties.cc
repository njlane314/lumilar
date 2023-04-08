#include "Signal.hh"
#include "MaterialProperties.hh"

MaterialProperties* MaterialProperties::instance_ = nullptr;

MaterialProperties::MaterialProperties(std::string material = "lAr") 
: material_(material) {
    if (!instance_) {
        instance_ = this;
        material_properties_ = initialise_properties(material_); 
    } 
}

MaterialProperties::~MaterialProperties() {}

MaterialProperties* MaterialProperties::get_instance() {
    if (!instance_) {
        instance_ = new MaterialProperties();
    }
    return instance_;
}

Properties* MaterialProperties::get_material_properties() {
    return &material_properties_;
}

void MaterialProperties::get_recombination_parameters(double& ARecomb, double& BRecomb, double& CRecomb) {
    ARecomb = 0.05 * std::pow(material_properties_.electric_field, -0.85);
    BRecomb = 0.06 * std::pow(material_properties_.electric_field, -0.85);
    CRecomb = 1/6;
}

Properties MaterialProperties::initialise_properties(std::string material = "lAr") {
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
    } 
    else if (material == "lArXe") {
        // change these values, and add Xe doping concentration
        material_properties.material = "lArXe";
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
    }
    return material_properties;
}