#include "AlphaGammaPhysics.hh"
#include "AlphaGammaProcess.hh"
#include "G4Alpha.hh"

AlphaGammaPhysics::AlphaGammaPhysics(const G4String& name) 
: G4VPhysicsConstructor(name) {}

AlphaGammaPhysics::~AlphaGammaPhysics() {}

void AlphaGammaPhysics::ConstructParticle() {}

void AlphaGammaPhysics::ConstructProcess() {
    G4Alpha::AlphaDefinition();

    AlphaGammaProcess* alphaGammaProcess = new AlphaGammaProcess();
    G4ProcessManager* pManager = G4Alpha::Alpha()->GetProcessManager();
    
    pManager->AddDiscreteProcess(alphaGammaProcess);
}
