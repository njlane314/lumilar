#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4RunManager.hh"

#include "DetectorConstruction.hh"
#include "MediumResponse.hh"
#include "HitDataHandler.hh"
#include "TruthManager.hh"
#include "Particle.hh"

#include "LArNEST.h"

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step);
    bool isParticleWithinDetector(const G4Step* step);
};

#endif // STEPPING_ACTION_HH
