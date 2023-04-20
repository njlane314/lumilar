#include "SteppingAction.hh"

SteppingAction::SteppingAction(bool signal_physics)
: signal_physics_(signal_physics) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
	if (signal_physics_ == true) {
		Signal::get_instance()->process_response(step);
	}
}