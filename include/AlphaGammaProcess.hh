#ifndef ALPHA_GAMMA_PROCESS_HH
#define ALPHA_GAMMA_PROCESS_HH

#include "G4ProcessManager.hh"
#include "AlphaGammaProcess.hh"
#include "G4Gamma.hh"
#include "G4ParticleChange.hh"
#include "G4DynamicParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4VDiscreteProcess.hh"

class AlphaGammaProcess : public G4VDiscreteProcess {
public:
    AlphaGammaProcess(const G4String& processName = "AlphaGammaCapture", G4ProcessType type = fElectromagnetic);
    virtual ~AlphaGammaProcess();

    virtual G4double GetMeanFreePath(const G4Track& aTrack, G4double previousStepSize, G4ForceCondition* condition) override;
    virtual G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) override;

private:
    G4ParticleChange aParticleChange;
};

#endif // ALPHA_GAMMA_PROCESS