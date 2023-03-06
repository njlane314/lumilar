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
        energy: 15.0,          // MeV
    },
  
    // Incident neutrino direction 3-vector
    direction: { x: 0.0, y: 0.0, z: 1.0 },
  
  }