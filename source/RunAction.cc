#include "RunAction.hh"
//_________________________________________________________________________________________
RunAction::RunAction() {}
//_________________________________________________________________________________________
RunAction::~RunAction() {}
//_________________________________________________________________________________________
void RunAction::BeginOfRunAction(const G4Run* run) {
	start_time_ = std::chrono::high_resolution_clock::now();

	std::cout << "-- Beginning run... " << std::endl;

	std::cout << "-- Constructing optical sensors... " << std::endl;
	InstrumentConstruction::GetInstance()->CreateOpticalSensors();

	const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
	for (const auto& sensor : optical_sensors) {
		std::cout << "Sensor position: (" << sensor->GetPosition().x() << ", " << sensor->GetPosition().y() << ", " << sensor->GetPosition().z() << ")\n";
		auto [width, height] = sensor->GetDimensions();
		std::cout << "Sensor dimensions: (" << width << ", " << height << ")\n";
	}

	this->HandleHits(run);
}
//_________________________________________________________________________________________
void RunAction::EndOfRunAction(const G4Run* run) {
	HitDataHandler::GetInstance()->Save();

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time_);
	std::cout << "-- Time taken to complete: " << duration_s.count() << " s" << std::endl;
}
//_________________________________________________________________________________________
void RunAction::HandleHits(const G4Run* run) {
	HitDataHandler* hit_data_handler = HitDataHandler::GetInstance();
	hit_data_handler->Book();
	hit_data_handler->SetRun(run->GetRunID());
	hit_data_handler->EventReset();
}