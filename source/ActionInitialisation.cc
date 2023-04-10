#include "ActionInitialisation.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

ActionInitialisation::ActionInitialisation() : G4VUserActionInitialization() {}

ActionInitialisation::~ActionInitialisation() {}

void ActionInitialisation::Build() const {
	SetUserAction(new PrimaryGeneratorAction());
	SetUserAction(new RunAction());
	SetUserAction(new EventAction());
	SetUserAction(new TrackingAction());
	SetUserAction(new SteppingAction());
}