#include "Signal.hh"
#include "MediumProperties.hh"
//_________________________________________________________________________________________
MediumProperties* MediumProperties::instance_ = nullptr;
//_________________________________________________________________________________________
MediumProperties::MediumProperties(std::string medium = "lAr") 
: medium_(medium) {
    if (!instance_) {
        instance_ = this;
        medium_properties_ = InitialiseProperties(medium_); 
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
Properties MediumProperties::InitialiseProperties(std::string medium = "lAr") {
    //https://github.com/LArSoft/lardataalg/blob/develop/lardataalg/DetectorInfo/larproperties.fcl
    Properties medium_properties = {};

    medium_properties.loss_per_ionisation = 23.6 * eV;
    medium_properties.ionisation_threshold = 15.4 * eV;
    medium_properties.excitation_threshold = 12.7 * eV;
    medium_properties.thermal_loss = 4.3 * eV;

    medium_properties.fano_factor = 0.107;
    medium_properties.excitation_ratio = 0.21;

    medium_properties.singlet_lifetime = 6. * ns;
    medium_properties.triplet_lifetime = 1590. * ns;

    medium_properties.muon_yield_quenching = 1;
    medium_properties.pion_yield_quenching = 1;
    medium_properties.electron_yield_quenching = 0.83;
    medium_properties.kaon_yield_quenching = 1;
    medium_properties.proton_yield_quenching = 0.8;
    medium_properties.alpha_yield_quenching = 0.7;

    medium_properties.muon_singlet_abundance = 0.19;
    medium_properties.pion_singlet_abundance = 0.19;
    medium_properties.electron_singlet_abundance = 0.21;
    medium_properties.kaon_singlet_abundance = 0.19;
    medium_properties.proton_singlet_abundance = 0.22;
    medium_properties.alpha_singlet_abundance = 0.36;

    //https://pubs.aip.org/aip/jcp/article/91/3/1469/220871/Argon-krypton-and-xenon-excimer-luminescence-From
    medium_properties.argon_spectra_wavelength_mean = 127.;
    medium_properties.argon_spectra_wavelength_sigma = 7.4 / (2 * sqrt(2 * log(2)));

    medium_properties.electric_field = 0.5; // kilovolt / cm
    
    return medium_properties;
}