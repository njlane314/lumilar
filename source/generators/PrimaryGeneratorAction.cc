#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
	generator_directory_ = new G4UIdirectory("/generator/");
  	generator_directory_->SetGuidance("Primary generator control commands.");

  	set_generator_cmd_ = new G4UIcmdWithAString("/generator/set", this);
  	set_generator_cmd_->SetGuidance("Sets the primary generator");
	set_generator_cmd_->SetParameterName("Generator", false, false);
	set_generator_cmd_->SetCandidates("marley general");

	marley_directory_ = new G4UIdirectory("/generator/marley/");
	marley_directory_->SetGuidance("Marley generator control commands");

	set_marley_source_cmd_ = new G4UIcmdWithAString("/generator/marley/source", this);
	set_marley_source_cmd_->SetGuidance("Sets the energy distribution");
	set_marley_source_cmd_->SetParameterName("Config", false, false);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {}

void PrimaryGeneratorAction::SetNewValue(G4UIcommand* cmd, G4String new_value) {
	if (generator_type_.empty()) {
		if (cmd == set_generator_cmd_) { 
			generator_type_ = new_value;
		}
		else {
			G4Exception("PrimaryGeneratorAction::SetNewValue()", "", FatalErrorInArgument, "Error: Please set the generator before calling this command");
		}
	}

	if (generator_type_ == "marley") {
		if (cmd == set_marley_source_cmd_) {
			marley_source_ = new_value;
			marley_generator_ = new MarleyGenerator(marley_source_);
		}
	}
	else if (generator_type_ == "general") {
		general_generator_ = new G4GeneralParticleSource();
	}
	else {
		G4Exception("PrimaryGeneratorAction::SetNewValue()", "", FatalErrorInArgument, "Error: Generator type not recognised");
	}

	if (!detector_solid_) {
		std::cout << "Detector dimensions not set, attempting to set them now" << std::endl;
		G4LogicalVolume* detector_logical = G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");
		if (detector_logical) detector_solid_ = dynamic_cast<G4Box*>(detector_logical->GetSolid());
	}
    
	if (detector_solid_) {
		detector_width_ = detector_solid_->GetXHalfLength() * 2.;
		detector_height_ = detector_solid_->GetYHalfLength() * 2.;
		detector_depth_ = detector_solid_->GetZHalfLength() * 2.;

		std::cout << "Detector dimensions: " << detector_width_ << " " << detector_height_ << " " << detector_depth_ << std::endl;
	} 
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
	if (generator_type_ == "marley") {
		marley_generator_->GeneratePrimaryVertex(event);
	}
	else if (generator_type_ == "general") {
		general_generator_->GeneratePrimaryVertex(event);
	}
	else {
		G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "", FatalErrorInArgument, "Error: Please enter a valid generator candidate");
	}
}