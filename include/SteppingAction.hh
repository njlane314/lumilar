#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"

#include "OutputManager.hh"
#include "Signal.hh"

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step);
    
    void PrintStep(const G4Step* step);

private:
    OutputManager* output_manager_;
    int verbose_level_;
};

#endif // STEPPING_ACTION_HH
