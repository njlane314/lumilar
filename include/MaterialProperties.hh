#ifndef MATERIAL_PROPERTIES_HH
#define MATERIAL_PROPERTIES_HH

#include "G4SystemOfUnits.hh"

#include <string> 
#include <vector>
#include <iostream>

struct Properties {
    double loss_per_ionisation;
    double ionisation_threshold;
    double excitation_threshold;
    double thermal_loss;

    double fano_factor;
    double excitation_ratio;

    double singlet_lifetime;
    double triplet_lifetime;

    double singlet_to_triplet_light;
    double singlet_to_triplet_heavy;

    double electric_field;
};

class MaterialProperties {
 public:
    MaterialProperties(std::string material);
    ~MaterialProperties();

    static MaterialProperties* get_instance();
    Properties initialise_properties(std::string material);
    Properties* get_material_properties();

    static MaterialProperties* instance_;
    std::string material_;
    Properties material_properties_;
};

#endif // MATERIAL_PROPERTIES_HH