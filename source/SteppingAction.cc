#include "SteppingAction.hh"

SteppingAction::SteppingAction() {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
	auto output_manager_ = OutputManager::Instance();
	output_manager_->RecordEntry(step);

	Signal::get_instance()->process_response(step);

}