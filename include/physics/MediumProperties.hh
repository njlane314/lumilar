#ifndef MEDIUM_PROPERTIES_HH
#define MEDIUM_PROPERTIES_HH

#include <string> 
#include <vector>
#include <iostream>
#include <map>

#include "G4SystemOfUnits.hh"

struct Properties {
    std::string medium;
    double loss_per_ionisation;
    double ionisation_threshold;
    double excitation_threshold;
    double thermal_loss;

    double fano_factor;
    double excitation_ratio;

    double singlet_lifetime;
    double triplet_lifetime;

    double muon_yield_quenching;
    double pion_yield_quenching;
    double electron_yield_quenching;
    double kaon_yield_quenching;
    double proton_yield_quenching;
    double alpha_yield_quenching;

    double muon_singlet_abundance;
    double pion_singlet_abundance;
    double electron_singlet_abundance;
    double kaon_singlet_abundance;
    double proton_singlet_abundance;
    double alpha_singlet_abundance;

    double electric_field;
};

class MediumProperties {
public:
    MediumProperties(std::string medium);
    ~MediumProperties();

    static MediumProperties* GetInstance();
    Properties InitialiseProperties(std::string medium);
    Properties* GetMediumProperties();

    static MediumProperties* instance_;
    std::string medium_;
    Properties medium_properties_;
};

#endif // MEDIUM_PROPERTIES_HH