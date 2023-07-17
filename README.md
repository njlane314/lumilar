# lumiLAr

```
      :::       :::    :::   :::   :::   ::::::::::: :::            :::     ::::::::: 
     :+:       :+:    :+:  :+:+: :+:+:      :+:     :+:          :+: :+:   :+:    :+: 
    +:+       +:+    +:+ +:+ +:+:+ +:+     +:+     +:+         +:+   +:+  +:+    +:+  
   +#+       +#+    +:+ +#+  +:+  +#+     +#+     +#+        +#++:++#++: +#++:++#:    
  +#+       +#+    +#+ +#+       +#+     +#+     +#+        +#+     +#+ +#+    +#+    
 #+#       #+#    #+# #+#       #+#     #+#     #+#        #+#     #+# #+#    #+#     
########## ########  ###       ### ########### ########## ###     ### ###    ###   
```

A simulation, using the Geant4 toolkit, that models the energy loss of neutrino-interaction particles, medium response and analytical optical transportation in liquid argon.

## Table of Contents

- [About](#about)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## About

The general outline of the simulation is as follows. 

An external event generator simulates the initial particle interaction, generating the "primary" tracks of an event. These are the tracks of the particles that are produced in the interaction. The event generators used in this simulation includes MARLEY, and BxDecay0. 

The particles are tracked through the detector geometry, where their interactions are sampled and can produce "secondary" tracks, or particles. Here, the "hit" information is stored at discrete steps along each particle's track. The simulation toolkit GEANT4 is used here for particle interactions and energy loss, with a simplified "box" geometrical description. 

The "hit" information is then processed, taking localised energy deposits and converting them into optical photon, and thermal electron radiation. A fast radiation transport simulation, based on analytical equations, is then used to propagate the photons to the photon detector readouts. 

## Getting Started

To get started with this software, follow the instructions below:

### Prerequisites
- [Root](https://github.com/root-project/root) can use cvmfs or install locally
- [Geant4](https://github.com/Geant4/geant4) can also use cvmfs or install locally

These can be installed in the local directory (i.e. ignore if wish and checkout build scripts):
- [Eigen](https://github.com/libigl/eigen) header-only library, can be included in the external directory
- [BxDecay0](https://github.com/BxCppDev/bxdecay0) can also be included in the external directory, or installed locally
- [Marley](https://github.com/MARLEY-MC/marley) included in the external directory, requires sourcing

### Installation

1. Clone the repository:
```shell
git clone https://github.com/njlane314/lumilar
```

2. Navigate to the project directory:
```shell
cd lumilar
```

3. Install dependencies and build: 
```shell
source setup_config.sh
./simple_build.sh -b -t
```

### Usage

Example usage: 
```shell
cd system
./lumilar -d detector/dune_tpc.mac -g generator/marley_solar.mac
```