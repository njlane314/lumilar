#include "ActionInitialisation.hh"
//_________________________________________________________________________________________
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"
//_________________________________________________________________________________________
ActionInitialisation::ActionInitialisation(std::string output_filename) : output_filename_(output_filename), G4VUserActionInitialization() {}
//_________________________________________________________________________________________
ActionInitialisation::~ActionInitialisation() {}
//_________________________________________________________________________________________
void ActionInitialisation::Build() const {
	SetUserAction(new PrimaryGeneratorAction(output_filename_));
	SetUserAction(new RunAction());
	SetUserAction(new EventAction());
	SetUserAction(new TrackingAction());
	SetUserAction(new SteppingAction());
}