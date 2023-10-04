#include "AlphaGammaProcess.hh"

AlphaGammaProcess::AlphaGammaProcess(const G4String& processName, G4ProcessType type)
: G4VDiscreteProcess(processName, type)
{}

AlphaGammaProcess::~AlphaGammaProcess()
{}

G4double AlphaGammaProcess::GetMeanFreePath(const G4Track& aTrack, G4double, G4ForceCondition* condition) {
    return 0.01*CLHEP::cm;
}

G4VParticleChange* AlphaGammaProcess::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep) {
    aParticleChange.Initialize(aTrack);
    
    aParticleChange.ProposeLocalEnergyDeposit(aTrack.GetKineticEnergy());

    G4double gammaEnergy = 15*CLHEP::MeV;
    G4ParticleDefinition* gamma = G4Gamma::GammaDefinition();
    aParticleChange.SetNumberOfSecondaries(1);

    G4ThreeVector gammaMomentumDirection = G4ThreeVector(0,1,0); 

    G4DynamicParticle* gammaParticle = new G4DynamicParticle(gamma, gammaMomentumDirection, gammaEnergy);
    aParticleChange.AddSecondary(gammaParticle);

    aParticleChange.ProposeTrackStatus(fStopAndKill);

    return &aParticleChange;
}
