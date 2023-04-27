#include "TrackingAction.hh"

TrackingAction::TrackingAction() {}

TrackingAction::~TrackingAction() {}

void TrackingAction::PreUserTrackingAction(const G4Track* track) {
    auto process = track->GetCreatorProcess();
    if (process != nullptr) {
        auto process_name = process->GetProcessName();
       
        if (process_name == "eIoni" || process_name == "hIoni" || process_name == "muIoni" || process_name == "ionIoni") {
            const_cast<G4Track*>(track)->SetTrackStatus(fStopAndKill);
        }
    } 
}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {}