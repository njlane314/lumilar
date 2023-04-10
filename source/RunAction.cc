#include "RunAction.hh"

RunAction::RunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
	start_time_ = std::chrono::high_resolution_clock::now();

	std::cout << "-- Beginning run... " << std::endl;
	
	new MaterialProperties("lAr");
	new SensorConstruction();

	std::cout << "-- Constructing optical sensors... " << std::endl;
	SensorConstruction::GetInstance()->ConstructRectangularOpticalSensors(PlaneOrientation::X_POS, 1.0, 0.5, 0.5);
	const OpticalSensorVector& optical_sensors = SensorConstruction::GetInstance()->GetOpticalSensors();

	for (const auto& sensor : optical_sensors) {
		std::cout << "Sensor position: (" << sensor->getPosition().x() << ", " << sensor->getPosition().y() << ", " << sensor->getPosition().z() << ")\n";
		auto [width, height] = sensor->getDimensions();
		std::cout << "Sensor dimensions: (" << width << ", " << height << ")\n";
	}

	std::cout << "Number of optical sensors: " << SensorConstruction::GetInstance()->GetNumOpticalSensors() << "\n";
}

void RunAction::EndOfRunAction(const G4Run* run) {


	auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time_);
    std::cout << "-- Time taken to complete: " << duration_s.count() << " s" << std::endl;
}