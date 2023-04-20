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

        material_properties.absorption_length = 20. * m;

        material_properties.muon_scintillation_yield = 24000.; // /MeV
        material_properties.muon_scintillation_ratio = 0.23; // fast / slow
        material_properties.pion_scintillation_yield = 24000.; // /MeV
        material_properties.pion_scintillation_ratio = 0.23; // fast / slow
        material_properties.electron_scintillation_yield = 20000.; // /MeV
        material_properties.electron_scintillation_ratio = 0.27; // fast / slow
        material_properties.kaon_scintillation_yield = 24000.; // /MeV
        material_properties.kaon_scintillation_ratio = 0.23; // fast / slow
        material_properties.proton_scintillation_yield = 19200.; // /MeV
        material_properties.proton_scintillation_ratio = 0.29; // fast / slow
        material_properties.alpha_scintillation_yield = 19200.;
        material_properties.alpha_scintillation_ratio = 0.56;
    } 
    //else if (material == "lArXe") {}
    
    return material_properties;
}


/* # For following parameters, see http://pdg.lbl.gov/AtomicNuclearProperties/
 RadiationLength:  19.55   # g/cm^2
 AtomicNumber:     18      # Ar atomic number.
 AtomicMass:       39.948  # Ar atomic mass (g/mol).
 ExcitationEnergy: 188.0   # Ar mean excitation energy (eV).

# Argon39DecayRate: 0.00141 # decays per cm^3 per second.  Assumes 1.01 Bq/kg and a density of 1.396 g/cc
# The following parameter was formerly used to steer an ad-hoc 39Ar decay model as part of the G4 step.
# Users are now recommended to use the radiological generator: see larsim/EventGenerators/radiological_gen.fcl
# As of February 2017, this value is no longer used.
 Argon39DecayRate: 0.0

 # Optical properties
 # Fast and slow scintillation emission spectra, from [J Chem Phys vol 91 (1989) 1469]
 FastScintEnergies:    [ 7.2,  7.9,  8.3,  8.6,  8.9,  9.1,  9.3,  9.6,  9.7,  9.8,  10,  10.2,  10.3,  10.6,  11,  11.6,  11.9]
 SlowScintEnergies:    [ 7.2,  7.9,  8.3,  8.6,  8.9,  9.1,  9.3,  9.6,  9.7,  9.8,  10,  10.2,  10.3,  10.6,  11,  11.6,  11.9]
 FastScintSpectrum:    [ 0.0,  0.04, 0.12, 0.27, 0.44, 0.62, 0.80, 0.91, 0.92, 0.85, 0.70, 0.50, 0.31, 0.13, 0.04,  0.01, 0.0]
 SlowScintSpectrum:    [ 0.0,  0.04, 0.12, 0.27, 0.44, 0.62, 0.80, 0.91, 0.92, 0.85, 0.70, 0.50, 0.31, 0.13, 0.04,  0.01, 0.0]
 ScintResolutionScale: 1.     # resolution factor used by G4 scintillation
 ScintFastTimeConst:   6.     # fast scintillation time constant (ns)
 ScintSlowTimeConst:   1590.  # slow scintillation time constant (ns)
 ScintBirksConstant:   0.069  # birks constant for LAr (1/MeV cm)
 ScintYield:           24000. # total scintillation yield (ph/Mev)
 ScintPreScale:        0.03   # Scale factor to reduce number of photons simulated
                              # Later QE should be corrected for this scale
 ScintYieldRatio:      0.3    # fast / slow scint ratio (needs revisitting)
 ScintByParticleType:  false  # whether to use different yields and
                              # quenching per particle in fast op sim
 EnableCerenkovLight: true    # whether to switch on cerenkov light (slow)




 # Scintillation yields and fast/slow ratios per particle type
 MuonScintYield:          24000
 MuonScintYieldRatio:     0.23
 PionScintYield:          24000
 PionScintYieldRatio:     0.23
 ElectronScintYield:      20000
 ElectronScintYieldRatio: 0.27
 KaonScintYield:          24000
 KaonScintYieldRatio:     0.23
 ProtonScintYield:        19200
 ProtonScintYieldRatio:   0.29
 AlphaScintYield:         16800
 AlphaScintYieldRatio:    0.56


 # Refractive index as a function of energy (eV) from arXiv:2002.09346
 RIndexEnergies: [ 1.18626, 1.68626, 2.18626, 2.68626, 3.18626, 3.68626, 4.18626, 4.68626, 5.18626, 5.68626, 6.18626, 6.68626, 7.18626, 7.68626, 8.18626, 8.68626, 9.18626, 9.68626, 10.1863, 10.6863, 11.1863 ]
 RIndexSpectrum: [ 1.24664, 1.2205, 1.22694, 1.22932, 1.23124, 1.23322, 1.23545, 1.23806, 1.24116, 1.24489, 1.24942, 1.25499, 1.26197, 1.2709, 1.28263, 1.29865, 1.32169, 1.35747, 1.42039, 1.56011, 2.16626 ]

 # absorption length as function of energy
 AbsLengthEnergies: [ 4,     5,     6,     7,     8,     9,     10,    11   ]
 AbsLengthSpectrum: [ 8000., 8000., 8000., 8000., 8000., 2000., 2000., 2000.]

 # Rayleigh scattering length (cm) @ 90K as a function of energy (eV) from arXiv:2002.09346

RayleighEnergies: [ 1.18626, 1.68626, 2.18626, 2.68626, 3.18626, 3.68626, 4.18626, 4.68626, 5.18626, 5.68626, 6.18626, 6.68626, 7.18626, 7.68626, 8.18626, 8.68626, 9.18626, 9.68626, 10.1863, 10.6863, 11.1863 ]
RayleighSpectrum: [ 1200800, 390747, 128633, 54969.1, 27191.8, 14853.7, 8716.9, 5397.42, 3481.37, 2316.51, 1577.63, 1092.02, 763.045, 534.232, 371.335, 252.942, 165.38, 99.9003, 51.2653, 17.495, 0.964341 ]

 # Surface reflectivity data - vector of energy spectrum per
 #   surface type
 ReflectiveSurfaceEnergies:           [ 7, 9, 10 ]
 ReflectiveSurfaceNames:            [ "STEEL_STAINLESS_Fe7Cr2Ni" ]
 ReflectiveSurfaceReflectances:     [ [ 0.25, 0.25, 0.25 ] ]
 ReflectiveSurfaceDiffuseFractions: [ [ 0.5,  0.5,  0.5  ] ]

 # Information related with the simulation of the Wavelength Shifter (TPB)
 LoadExtraMatProperties: false

 # TPB - WLS
 TpbTimeConstant: 2.5 #wls time constant in s J. Lumin 81(1999) 285

 # WLS - TPB properties original tpb [0.0, 0.0, 0.0, 0.0588,0.235, 0.853, 1.0,1.0,0.9259,0.704,0.0296,0.011, 0.0,0.0, 0.]
 TpbEmmisionEnergies: [0.05,1.0,1.5, 2.25, 2.481, 2.819, 2.952,2.988,3.024, 3.1, 3.14,3.1807, 3.54, 5.5, 50.39]
 TpbEmmisionSpectrum: [0.0, 0.0, 0.0, 0.0588,0.235, 0.853, 1.0,1.0,0.9259,0.704,0.0296,0.011, 0.0,0.0, 0.]
 TpbAbsorptionEnergies: [0.05,1.77,2.0675, 7.42, 7.75, 8.16, 8.73, 9.78,10.69, 50.39]
 TpbAbsorptionSpectrum: [100000.0,100000.0, 100000.0,0.001,0.00000000001,0.00000000001, 0.00000000001, 0.00000000001, 0.00000000001, 0.00000000001]

}
*/