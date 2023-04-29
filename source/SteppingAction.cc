#include "SteppingAction.hh"

SteppingAction::SteppingAction(bool is_signal_physics)
: is_signal_physics_(is_signal_physics) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
	if (is_signal_physics_ == true) {
		MediumResponse::ProcessResponse(step);
	}
}