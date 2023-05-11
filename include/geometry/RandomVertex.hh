class RandomVertex : public G4PrimaryVertex {
public:
    RandomVertex() : G4PrimaryVertex() {};
	~RandomVertex() {};

	static G4PrimaryVertex* Generate() {
		double detector_width, detector_height, detector_depth;
		DetectorConstruction* detector_construction = (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
		detector_construction->GetDetectorDimensions(detector_width, detector_height, detector_depth);

		double x_pos = CLHEP::RandFlat::shoot(-detector_width/2, detector_width/2);
		double y_pos = CLHEP::RandFlat::shoot(-detector_height/2, detector_height/2);
		double z_pos = CLHEP::RandFlat::shoot(-detector_depth/2, detector_depth/2);

		G4ThreeVector position = G4ThreeVector(x_pos, y_pos, z_pos);
		this->SetPosition(position);

		return this;
	}
};
