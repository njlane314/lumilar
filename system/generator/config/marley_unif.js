// Use this example job configuration file as a starting point for your own
// files.
{
    // Pure 40Ar target
    target: {
      nuclides: [ 1000180400 ],
      atom_fractions: [ 1.0 ],
    },
  
    // Simulate CC ve scattering on 40Ar
    reactions: [ "ve40ArCC_Bhattacharya2009.react" ],
    
    source: {
        type: "histogram",
        neutrino: "ve",
        E_bin_lefts: [ 0.],   // Low edges of energy bins (MeV)
        weights: [ 1. ],       // Bin weights (dimensionless)
        Emax: 30.,                        // Upper edge of the final bin (MeV)
        weight_flux: false,
    },
  
    // Incident neutrino direction 3-vector
    direction: "isotropic",

    log: [ { file: "stdout", level: "disabled" } ],
}