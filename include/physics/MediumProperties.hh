#ifndef MEDIUM_PROPERTIES_HH
#define MEDIUM_PROPERTIES_HH

#include "G4SystemOfUnits.hh"

#include <string> 
#include <vector>
#include <iostream>

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

    double argon_spectra_wavelength_mean;
    double argon_spectra_wavelength_sigma;

    double xenon_spectra_wavelength_mean;
    double xenon_spectra_wavelength_sigma;

    double electric_field;

    double doped_concentration;

    bool is_doped;
    bool quenching_enabled;
};

class MediumProperties {
public:
    MediumProperties(std::string medium);
    ~MediumProperties();

    static MediumProperties* GetInstance();
    Properties InitialiseProperties(std::string medium);
    Properties* GetMediumProperties();
    double GetGroupVelocity(double wavelength) const;

    static MediumProperties* instance_;
    std::string medium_;
    Properties medium_properties_;
};

#endif // MEDIUM_PROPERTIES_HH