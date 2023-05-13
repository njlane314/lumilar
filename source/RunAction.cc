#include "RunAction.hh"
//_________________________________________________________________________________________
RunAction::RunAction() {}
//_________________________________________________________________________________________
RunAction::~RunAction() {}
//_________________________________________________________________________________________
void RunAction::BeginOfRunAction(const G4Run* run) {
	start_time_ = std::chrono::high_resolution_clock::now();

	std::cout << "-- Beginning run... " << std::endl;
	
	new MediumProperties("lAr");

	std::cout << "-- Constructing optical sensors... " << std::endl;
	//double rectangular_unif = 1000.; // mm
	//InstrumentConstruction::GetInstance()->ConstructRectangularOpticalSensors(PlaneOrientation::X_POS, rectangular_unif, rectangular_unif, rectangular_unif, rectangular_unif);
	InstrumentConstruction::GetInstance()->ConstructRectangularOpticalSensors(PlaneOrientation::X_POS, 60000, 12000, 60000, 12000);

	const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
	for (const auto& sensor : optical_sensors) {
		std::cout << "Sensor position: (" << sensor->GetPosition().x() << ", " << sensor->GetPosition().y() << ", " << sensor->GetPosition().z() << ")\n";
		auto [width, height] = sensor->GetDimensions();
		std::cout << "Sensor dimensions: (" << width << ", " << height << ")\n";
	}
}
//_________________________________________________________________________________________
void RunAction::EndOfRunAction(const G4Run* run) {
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time_);
	std::cout << "-- Time taken to complete: " << duration_s.count() << " s" << std::endl;
}