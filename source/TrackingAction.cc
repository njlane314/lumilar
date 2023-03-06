#include "TrackingAction.hh"

TrackingAction::TrackingAction()
: output_manager_(nullptr), verbose_level_(0) {
	output_manager_ = OutputManager::Instance();
}

TrackingAction::~TrackingAction() {}

void TrackingAction::PreUserTrackingAction(const G4Track* track) {
    auto process = track->GetCreatorProcess();
    if (process != nullptr) {
        auto process_name = process->GetProcessName();
        auto kinetic_energy = track->GetKineticEnergy() * MeV;
       
        if (process_name == "eIoni" || process_name == "hIoni" || process_name == "muIoni" || process_name == "ionIoni") {
            const_cast<G4Track*>(track)->SetTrackStatus(fStopAndKill);
        }
    } 
}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {
	if (verbose_level_ > 1) {
		PrintTrack(track);
	}

	output_manager_->RecordEntry(track);
}

void TrackingAction::PrintTrack(const G4Track* track) {}