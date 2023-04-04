#include "RunAction.hh"

RunAction::RunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
	start_time_ = std::chrono::high_resolution_clock::now();

	std::cout << "-- Beginning run... " << std::endl;
	
	new AnalysisManager();
	new OutputManager();
	new MaterialProperties("lAr");

	OutputManager::Instance()->CreateFile();
}

void RunAction::EndOfRunAction(const G4Run* run) {
	AnalysisManager::Instance()->SaveFile();

	OutputManager::Instance()->RecordEntry(run);
	OutputManager::Instance()->SaveFile();

	auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time_);
    std::cout << "-- Time taken to complete: " << duration_s.count() << " s" << std::endl;
}