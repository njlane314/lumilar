{
    // Pure 40Ar target
    target: {
      nuclides: [ 1000180400 ],
      atom_fractions: [ 1.0 ],
    },
  
    // Simulate CC ve scattering on 40Ar
    reactions: [ "ve40ArCC_Bhattacharya2009.react" ],
    
    //  Neutrino source specification
    source: {
        neutrino: "ve",        // The source produces electron neutrinos
        type: "monoenergetic",
        energy: 8.0,          // MeV
        weight_flux: false,
    },

    direction: "isotropic",
  
    log: [ { file: "stdout", level: "disabled" } ],
  }