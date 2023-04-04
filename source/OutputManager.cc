#include "OutputManager.hh"

OutputManager* OutputManager::instance_ = nullptr;

OutputManager::OutputManager() {}

OutputManager::~OutputManager() {}

OutputManager* OutputManager::Instance() {
    if (!instance_) {
        instance_ = new OutputManager();
    }
    
    return instance_;
}

void OutputManager::CreateFile() {
    filename_ = "output.root";
    std::cout << "-- Creating output file: " << filename_ << std::endl;
    file_ = new TFile(filename_.c_str(), "RECREATE");

    meta_tree_ = new TTree("meta_tree", "Meta tree");
    meta_tree_->Branch("detector_width", &detector_width, "detector_width/D");
    meta_tree_->Branch("detector_height", &detector_height, "detector_height/D");
    meta_tree_->Branch("detector_depth", &detector_depth, "detector_depth/D");

    event_tree_ = new TTree("event_tree", "Event tree");
    event_tree_->Branch("track_count", &track_count, "track_count/I");
    event_tree_->Branch("step_count", &step_count, "step_count/I");

    track_tree_ = new TTree("track_tree", "Track tree");
    track_tree_->Branch("parent_track_id", &parent_track_id, "parent_track_id/I");
    track_tree_->Branch("track_particle", &track_particle, "track_particle/I");
    track_tree_->Branch("track_position_x", &track_position_x, "track_position_x/D");
    track_tree_->Branch("track_position_y", &track_position_y, "track_position_y/D");
    track_tree_->Branch("track_position_z", &track_position_z, "track_position_z/D");
    track_tree_->Branch("track_momentum_x", &track_momentum_x, "track_momentum_x/D");
    track_tree_->Branch("track_momentum_y", &track_momentum_y, "track_momentum_y/D");
    track_tree_->Branch("track_momentum_z", &track_momentum_z, "track_momentum_z/D");
    track_tree_->Branch("track_time", &track_time, "track_time/D");
    track_tree_->Branch("track_total_energy", &track_total_energy, "track_total_energy/D");
    track_tree_->Branch("track_kinetic_energy", &track_kinetic_energy, "track_kinetic_energy/D");
    track_tree_->Branch("track_length", &track_length, "track_length/D");

    step_tree_ = new TTree("step_tree", "Step tree");
    step_tree_->Branch("step_track_id", &step_track_id, "step_track_id/I");
    step_tree_->Branch("step_length", &step_length, "step_length/D");
    step_tree_->Branch("step_energy_deposit", &step_energy_deposit, "step_energy_deposit/D");
    step_tree_->Branch("step_non_ionizing_energy_deposit", &step_non_ionizing_energy_deposit, "step_non_ionizing_energy_deposit/D");
    step_tree_->Branch("step_position_x", &step_position_x, "step_position_x/D");
    step_tree_->Branch("step_position_y", &step_position_y, "step_position_y/D");
    step_tree_->Branch("step_position_z", &step_position_z, "step_position_z/D");
    step_tree_->Branch("step_momentum_x", &step_momentum_x, "step_momentum_x/D");
    step_tree_->Branch("step_momentum_y", &step_momentum_y, "step_momentum_y/D");
    step_tree_->Branch("step_momentum_z", &step_momentum_z, "step_momentum_z/D");
    step_tree_->Branch("step_time", &step_time, "step_time/D");
    step_tree_->Branch("step_total_energy", &step_total_energy, "step_total_energy/D");
    step_tree_->Branch("step_kinetic_energy", &step_kinetic_energy, "step_kinetic_energy/D");

    signal_tree_ = new TTree("signal_tree", "Photon tree");
    signal_tree_->Branch("photon_time", &photon_time, "photon_time/D");
    signal_tree_->Branch("photon_number", &photon_num, "photon_num/I");
    signal_tree_->Branch("electron_number", &electron_num, "electron_num/I");
}

void OutputManager::SaveFile() {
    if (!file_) {
        return;
    }
    else {
        file_->cd();
        
        meta_tree_->Write();
        event_tree_->Write();
        track_tree_->Write();
        step_tree_->Write();
        signal_tree_->Write();

        file_->Close();
        std::cout << "-- Output done" << std::endl;
        delete file_;
    }
}

void OutputManager::RecordEntry(const G4Run* run) {
    if (!detector_solid_) {
		G4LogicalVolume* detector_logical = G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");
		if (detector_logical) detector_solid_ = dynamic_cast<G4Box*>(detector_logical->GetSolid());
	}
    else if (detector_solid_) {
        detector_width = detector_solid_->GetXHalfLength() * 2.;
		detector_height = detector_solid_->GetYHalfLength() * 2.;
		detector_depth = detector_solid_->GetZHalfLength() * 2.;
	}
    else {
        detector_width = 0.;
        detector_height = 0.;
        detector_depth = 0.;
    }

    meta_tree_->Fill();
}

void OutputManager::RecordEntry(const G4Event* event) {
    event_tree_->Fill();

    track_count = 0;
    step_count = 0;
}

void OutputManager::RecordEntry(const G4Track* track) {
    parent_track_id = track->GetParentID();
    track_particle = track->GetParticleDefinition()->GetPDGEncoding();
    track_position_x = track->GetPosition().x();
    track_position_y = track->GetPosition().y();
    track_position_z = track->GetPosition().z();
    track_momentum_x = track->GetMomentum().x();
    track_momentum_y = track->GetMomentum().y();
    track_momentum_z = track->GetMomentum().z();
    track_time = track->GetGlobalTime();
    track_total_energy = track->GetTotalEnergy();
    track_kinetic_energy = track->GetKineticEnergy();
    track_length = track->GetTrackLength();

    track_tree_->Fill();

    track_count++;
}

void OutputManager::RecordEntry(const G4Step* step) {
    step_track_id = step->GetTrack()->GetTrackID();
    step_length = step->GetStepLength();
    step_energy_deposit = step->GetTotalEnergyDeposit();
    step_non_ionizing_energy_deposit = step->GetNonIonizingEnergyDeposit();
    step_position_x = step->GetPreStepPoint()->GetPosition().x();
    step_position_y = step->GetPreStepPoint()->GetPosition().y();
    step_position_z = step->GetPreStepPoint()->GetPosition().z();
    step_momentum_x = step->GetPreStepPoint()->GetMomentum().x();
    step_momentum_y = step->GetPreStepPoint()->GetMomentum().y();
    step_momentum_z = step->GetPreStepPoint()->GetMomentum().z();
    step_time = step->GetPreStepPoint()->GetGlobalTime();
    step_total_energy = step->GetPreStepPoint()->GetTotalEnergy();
    step_kinetic_energy = step->GetPreStepPoint()->GetKineticEnergy();
  
    step_tree_->Fill();

    step_count++;
}

void OutputManager::RecordEntry(const Scintillation* scintillation, const Ionisation* ionisation) {
    std::vector<double> emission_times = scintillation->get_emission_times();
    
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();

    for (int i = 0; i < emission_times.size(); i++) {
        photon_time = emission_times[i];
        signal_tree_->Fill();
    }

    if (radiant_sizes.size() == cloud_sizes.size()) {
        for (int i = 0; i < radiant_sizes.size(); i++) {
            photon_num = radiant_sizes[i];
            electron_num = cloud_sizes[i];
            signal_tree_->Fill();
        }
    } else {
        abort();
    }
}