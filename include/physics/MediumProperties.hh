//____________________________________________________________________________
/*!

\class   physics::MediumProperties

\brief   This class contains the properties of the detector medium.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

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

    double xenon_concentration;
    double nitrogen_concentration;

    double excited_rate;
    double quenched_rate;

    double transfer_rate; 

    double argon_spectra_wavelength_mean;
    double argon_spectra_wavelength_sigma;

    double xenon_spectra_wavelength_mean;
    double xenon_spectra_wavelength_sigma;

    std::vector<double> xe_scint_profile;

    double electric_field;
};

class MediumProperties {
public:
    MediumProperties(double xenon_concentration, double nitrogen_concentration);
    ~MediumProperties();

    static MediumProperties* GetInstance();
    Properties InitialiseProperties();
    std::vector<double> ParameteriseXeScintillationProfile(double triplet_lifetime, double excited_rate, double transfer_rate, double doped_concentration);
    Properties* GetMediumProperties();
    double GetGroupVelocity(double wl) const;
    double GetRefractiveIndex(double wl) const;
    double GetRayleighScatteringLength(double wl) const;
    double GetAbsorptionLength() const;

    static MediumProperties* instance_;
    std::string medium_;
    Properties medium_properties_;

    double nitrogen_concentration_;
    double xenon_concentration_;
};

#endif // MEDIUM_PROPERTIES_HH