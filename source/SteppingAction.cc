#include "SteppingAction.hh"

SteppingAction::SteppingAction() {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
	
	Signal::get_instance()->process_response(step);

}