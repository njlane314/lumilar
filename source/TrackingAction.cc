#include "TrackingAction.hh"
//_________________________________________________________________________________________
TrackingAction::TrackingAction() {}
//_________________________________________________________________________________________
TrackingAction::~TrackingAction() {}
//_________________________________________________________________________________________
void TrackingAction::PreUserTrackingAction(const G4Track* track) {
    /*auto process = track->GetCreatorProcess();
    if (process != nullptr) {
        auto process_name = process->GetProcessName();
       
        if (process_name == "eIoni") {
            const_cast<G4Track*>(track)->SetTrackStatus(fStopAndKill);
        }
    }*/
}
//_________________________________________________________________________________________
void TrackingAction::PostUserTrackingAction(const G4Track* track) {}