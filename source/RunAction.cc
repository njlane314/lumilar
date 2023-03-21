#include "RunAction.hh"

RunAction::RunAction() 
: output_manager_(nullptr), verbose_level_(0) {
	output_manager_ = OutputManager::Instance();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
	MaterialProperties* material_properties = new MaterialProperties("lAr");

	start_time_ = std::chrono::high_resolution_clock::now();
}

void RunAction::EndOfRunAction(const G4Run* run) {
	if (verbose_level_ > 0) {
		PrintRun();
	}

	output_manager_->RecordEntry(run);
	output_manager_->CloseFile();

	auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time_);
    std::cout << "-- Time taken to complete: " << duration_s.count() << " s" << std::endl;
}

void RunAction::PrintRun() {}