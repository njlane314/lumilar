#include "SteppingAction.hh"
//_________________________________________________________________________________________
SteppingAction::SteppingAction() {}
//_________________________________________________________________________________________
SteppingAction::~SteppingAction() {}
//_________________________________________________________________________________________
void SteppingAction::UserSteppingAction(const G4Step* step) {
	if (isParticleWithinDetector(step) == true) {
		larnest::LArNESTResult medium_response = MediumResponse::ProcessResponse(step);
	}
}
//_________________________________________________________________________________________
bool SteppingAction::isParticleWithinDetector(const G4Step* step) {
	double detector_width, detector_height, detector_depth;
	DetectorConstruction* detector_construction = (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	detector_construction->GetDetectorDimensions(detector_width, detector_height, detector_depth);

	double x_pos = step->GetPostStepPoint()->GetPosition().x();
	double y_pos = step->GetPostStepPoint()->GetPosition().y();
	double z_pos = step->GetPostStepPoint()->GetPosition().z();

	if (x_pos > detector_width / 2. || x_pos < -detector_width / 2. || y_pos > detector_height / 2. || y_pos < -detector_height / 2. || z_pos > detector_depth / 2. || z_pos < -detector_depth / 2.) {
		step->GetTrack()->SetTrackStatus(fStopAndKill);
		return false;
	} else {
		return true;
	}
}