#include "RunAction.hh"

RunAction::RunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
	start_time_ = std::chrono::high_resolution_clock::now();

	std::cout << "-- Beginning run..." << std::endl;
	std::cout << "-- Run ID: " << run->GetRunID() << std::endl;
	
	new AnalysisManager();
	new OutputManager();
	new MaterialProperties("lAr");
	new PulseShapeOutput();
	new SensorConstruction();

	std::cout << "-- Constructing optical sensors... " << std::endl;
	SensorConstruction::GetInstance()->ConstructRectangularOpticalSensors(PlaneOrientation::X_POS, 1.0, 0.5, 0.5);
	const OpticalSensorVector& optical_sensors = SensorConstruction::GetInstance()->GetOpticalSensors();

	for (const auto& sensor : optical_sensors) {
		std::cout << "Sensor position: (" << sensor->getPosition().x() << ", " << sensor->getPosition().y() << ", " << sensor->getPosition().z() << ")\n";
		std::cout << "Sensor orientation: " << static_cast<int>(sensor->getOrientation()) << "\n";
		std::cout << "\n";
	}

	OutputManager::Instance()->CreateFile();
}

void RunAction::EndOfRunAction(const G4Run* run) {
	AnalysisManager::Instance()->SaveFile();

	OutputManager::Instance()->RecordEntry(run);
	OutputManager::Instance()->SaveFile();

	PulseShapeOutput::Instance()->SaveFile();

	auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time_);
    std::cout << "-- Time taken to complete: " << duration_s.count() << " s" << std::endl;
}