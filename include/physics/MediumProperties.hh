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

    double singlet_abundance;
    double triplet_abundance;

    double absorption_length;

    double excited_rate;
    double quenched_rate;

    double transfer_rate; 

    double argon_spectra_wavelength_mean;
    double argon_spectra_wavelength_sigma;

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