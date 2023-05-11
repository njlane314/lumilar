#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4RunManager.hh"

#include "DetectorConstruction.hh"
#include "MediumResponse.hh"

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step);
    bool IsParticleWithinDetector(const G4Step* step);
};

#endif // STEPPING_ACTION_HH
