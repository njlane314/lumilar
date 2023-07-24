#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronElasticPhysicsHP.hh"

#include "QGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"

class PhysicsList : public G4VModularPhysicsList{
public:
    PhysicsList();
    ~PhysicsList();

private:
    std::string physics_list_;
};

#endif // PHYSICS_LIST_HH

