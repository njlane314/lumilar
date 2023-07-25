#ifndef NEUTRONPHYSICS_HH
#define NEUTRONPHYSICS_HH 

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class G4GenericMessenger;

class NeutronPhysics : public G4VPhysicsConstructor {
public:
    NeutronPhysics(const G4String& name="neutron");
    ~NeutronPhysics() override;

public:
    void ConstructParticle() override { };
    void ConstructProcess()  override;
    
private:
    void DefineCommands();
      
    G4bool              fThermal = true;
    G4GenericMessenger* fMessenger = nullptr;
};

#endif // NEUTRONPHYSICS_HH