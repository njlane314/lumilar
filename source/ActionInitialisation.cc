#include "ActionInitialisation.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

ActionInitialisation::ActionInitialisation(std::string output_filename) 
: output_filename_(output_filename), G4VUserActionInitialization() {}

ActionInitialisation::~ActionInitialisation() {}

void ActionInitialisation::Build() const {
	bool signal_physics = true;

	SetUserAction(new PrimaryGeneratorAction(output_filename_));
	SetUserAction(new RunAction());
	SetUserAction(new EventAction(signal_physics));
	SetUserAction(new TrackingAction());
	SetUserAction(new SteppingAction(signal_physics));
}