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
        E_bin_lefts: [ 40., ],   // Low edges of energy bins (MeV)
        weights: [ 1 ],       // Bin weights (dimensionless)
        Emax: 50.,                        // Upper edge of the final bin (MeV)
    },
  
    // Incident neutrino direction 3-vector
    direction: { x: 0.0, y: 0.0, z: 1.0 },
  
  }