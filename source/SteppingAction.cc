#include "SteppingAction.hh"

SteppingAction::SteppingAction() 
: output_manager_(nullptr), verbose_level_(0) {
	output_manager_ = OutputManager::Instance();
}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    if (verbose_level_ > 2) {
		PrintStep(step);
	}

	output_manager_->RecordEntry(step);

	Signal::get_instance()->process_response(step);
}

void SteppingAction::PrintStep(const G4Step* step) {}