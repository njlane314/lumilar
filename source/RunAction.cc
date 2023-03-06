#include "RunAction.hh"

RunAction::RunAction() 
: output_manager_(nullptr), verbose_level_(0) {
	output_manager_ = OutputManager::Instance();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {}

void RunAction::EndOfRunAction(const G4Run* run) {
	if (verbose_level_ > 0) {
		PrintRun();
	}

	output_manager_->RecordEntry(run);
	output_manager_->CloseFile();
}

void RunAction::PrintRun() {}