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
	double rectangular_unif = 300.; // mm
	InstrumentConstruction::GetInstance()->ConstructRectangularOpticalSensors(PlaneOrientation::X_POS, rectangular_unif, rectangular_unif, rectangular_unif, rectangular_unif);
	//InstrumentConstruction::GetInstance()->ConstructRectangularOpticalSensors(PlaneOrientation::X_POS, 60000, 12000, 60000, 12000);

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
	this->FillMetaData();
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

	TruthManager* truth_manager = TruthManager::GetInstance();
	truth_manager->EventReset();
}
//_________________________________________________________________________________________
void RunAction::FillMetaData() {
	HitDataHandler* hit_data_handler = HitDataHandler::GetInstance();
	G4LogicalVolume* detector_logic_vol
      = G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");
    if (detector_logic_vol) {
      G4Box * detector_solid_vol 
	   = dynamic_cast<G4Box*>(detector_logic_vol->GetSolid());

		double const detector_length_x = detector_solid_vol->GetXHalfLength() * 2. / CLHEP::cm;
		double const detector_length_y = detector_solid_vol->GetYHalfLength() * 2. / CLHEP::cm;
		double const detector_length_z = detector_solid_vol->GetZHalfLength() * 2. / CLHEP::cm;

		hit_data_handler->FillMetadata(detector_length_x,
										detector_length_y,
										detector_length_z);
    }
}