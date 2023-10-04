#ifndef AlphaGammaPhysics_hh
#define AlphaGammaPhysics_hh

#include "G4VPhysicsConstructor.hh"
#include "G4VDiscreteProcess.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleChange.hh"

class AlphaGammaPhysics : public G4VPhysicsConstructor
{
public:
    AlphaGammaPhysics(const G4String& name = "AlphaGamma");
    virtual ~AlphaGammaPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();
};

#endif