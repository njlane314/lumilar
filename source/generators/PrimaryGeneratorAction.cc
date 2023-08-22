#include "PrimaryGeneratorAction.hh"
//_________________________________________________________________________________________
PrimaryGeneratorAction::PrimaryGeneratorAction(std::string output_filename)
: output_filename_(output_filename) {
	generator_directory_ = new G4UIdirectory("/generator/");
  	generator_directory_->SetGuidance("Primary generator control commands.");

  	set_generator_cmd_ = new G4UIcmdWithAString("/generator/set", this);
  	set_generator_cmd_->SetGuidance("Sets the primary generator");
	set_generator_cmd_->SetParameterName("Generator", false, false);
	set_generator_cmd_->SetCandidates("marley general bxdecay0");

	marley_directory_ = new G4UIdirectory("/generator/marley/");
	marley_directory_->SetGuidance("Marley generator control commands");

	set_marley_source_cmd_ = new G4UIcmdWithAString("/generator/marley/source", this);
	set_marley_source_cmd_->SetGuidance("Sets the energy distribution");
	set_marley_source_cmd_->SetParameterName("Config", false, false);

	bxdecay0_directory_ = new G4UIdirectory("/generator/bxdecay0/");
	bxdecay0_directory_->SetGuidance("BxDecay0 generator control commands");

	fixed_position_directory_ = new G4UIdirectory("/generator/position/fixed/");
	fixed_position_directory_->SetGuidance("Fixed position generator control commands");

	set_fixed_position_cmd_ = new G4UIcmdWith3Vector("/generator/position/fixed/set", this);
	set_fixed_position_cmd_->SetGuidance("Sets the fixed position of the generator");
	set_fixed_position_cmd_->SetParameterName("x", "y", "z", false, false);
}
//_________________________________________________________________________________________
PrimaryGeneratorAction::~PrimaryGeneratorAction() {}
//_________________________________________________________________________________________
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
			marley_generator_ = new MarleyGenerator(marley_source_, output_filename_);
		}
	}
	else if (generator_type_ == "bxdecay0") {
		bxdecay0_generator_ = new BxDecay0Generator::PrimaryGeneratorAction();
	}
	else if (generator_type_ == "general") {
		general_generator_ = new G4GeneralParticleSource();
	}
	else {
		G4Exception("PrimaryGeneratorAction::SetNewValue()", "", FatalErrorInArgument, "Error: Generator type not recognised");
	}

	if (cmd == set_fixed_position_cmd_) {
		fixed_position_ = set_fixed_position_cmd_->GetNew3VectorValue(new_value);
		use_fixed_position_ = true;
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
//_________________________________________________________________________________________
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
	if (use_fixed_position_) {
        if (generator_type_ == "marley") {
            marley_generator_->SetPosition(fixed_position_);
        }
        else if (generator_type_ == "bxdecay0") {
            bxdecay0_generator_->SetPosition(fixed_position_);
        }
    }

	if (generator_type_ == "marley") {
		marley_generator_->GeneratePrimaryVertex(event);
	}
	else if (generator_type_ == "bxdecay0") {
		bxdecay0_generator_->GeneratePrimaries(event);
	}
	else if (generator_type_ == "general") {
		general_generator_->GeneratePrimaryVertex(event);
	}
	else {
		G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "", FatalErrorInArgument, "Error: Please enter a valid generator candidate");
	}
}