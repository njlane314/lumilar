#include "RunAction.hh"

RunAction::RunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
	start_time_ = std::chrono::high_resolution_clock::now();

	std::cout << "-- Beginning run... " << std::endl;
	auto analysis_manager = new AnalysisManager();
	auto output_manager = new OutputManager();
	
	auto material_properties = new MaterialProperties("lAr");
}

void RunAction::EndOfRunAction(const G4Run* run) {
	auto analysis_manager = AnalysisManager::Instance();
	analysis_manager->SaveFile();

	auto output_manager = OutputManager::Instance();
	output_manager->RecordEntry(run);
	output_manager->SaveFile();

	auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time_);
    std::cout << "-- Time taken to complete: " << duration_s.count() << " s" << std::endl;
}

void RunAction::PrintRun() {}