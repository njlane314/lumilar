#ifndef MATERIAL_PROPERTIES_HH
#define MATERIAL_PROPERTIES_HH

#include "G4SystemOfUnits.hh"

#include <string> 
#include <vector>
#include <iostream>

struct Properties {
    std::string material;
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

    double excited_rate_light;
    double quenched_rate_light;

    double excited_rate_heavy;
    double quenched_rate_heavy;

    double absorption_length;
};

class MaterialProperties {
public:
    MaterialProperties(std::string material);
    ~MaterialProperties();

    static MaterialProperties* getInstance();
    Properties InitialiseProperties(std::string material);
    Properties* getMaterialProperties();

    static MaterialProperties* instance_;
    std::string material_;
    Properties material_properties_;
};

#endif // MATERIAL_PROPERTIES_HH