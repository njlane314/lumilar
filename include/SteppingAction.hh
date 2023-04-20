#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"

#include "Signal.hh"

class SteppingAction : public G4UserSteppingAction {
 public:
    SteppingAction(bool signal_physics);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step);
 private:
    bool signal_physics_;
};

#endif // STEPPING_ACTION_HH
