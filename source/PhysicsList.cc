#include "PhysicsList.hh"
//_________________________________________________________________________________________
PhysicsList::PhysicsList() {
	G4StepLimiterPhysics* step_limit_phys = new G4StepLimiterPhysics();
	step_limit_phys->SetApplyToAll(true);
	RegisterPhysics(step_limit_phys);

	RegisterPhysics( new G4DecayPhysics() );
	RegisterPhysics( new G4EmStandardPhysics_option4() );
	RegisterPhysics( new G4EmExtraPhysics() );
	RegisterPhysics( new G4HadronElasticPhysics() );
	RegisterPhysics( new G4HadronPhysicsQGSP_BIC() );
	RegisterPhysics( new G4StoppingPhysics() );
	RegisterPhysics( new G4IonElasticPhysics() );
	RegisterPhysics( new G4IonPhysics() );

    RegisterPhysics( new NeutronPhysics("neutronHP"));  
}
//_________________________________________________________________________________________
PhysicsList::~PhysicsList() {}