#include "TrackingAction.hh"
//_________________________________________________________________________________________
TrackingAction::TrackingAction() {}
//_________________________________________________________________________________________
TrackingAction::~TrackingAction() {}
//_________________________________________________________________________________________
void TrackingAction::PreUserTrackingAction(const G4Track* track) {
    auto process = track->GetCreatorProcess();
    if (process != nullptr) {
        auto process_name = process->GetProcessName();
       
        if (process_name == "eIoni") {
            const_cast<G4Track*>(track)->SetTrackStatus(fStopAndKill);
        }
    } 

    TruthManager* truth_manager = TruthManager::GetInstance();

    Particle* particle = new Particle();
    particle->SetTrackID(track->GetTrackID());
    particle->SetParentTrackID(track->GetParentID());
    particle->SetPDGCode(track->GetDefinition()->GetPDGEncoding());
    particle->SetMass(track->GetDynamicParticle()->GetMass());
    particle->SetCharge(track->GetDynamicParticle()->GetCharge());
    particle->SetGlobalTime(track->GetGlobalTime() / CLHEP::ns);
    particle->SetTotalOccupancy(track->GetDynamicParticle()->GetTotalOccupancy());

    particle->SetInitialPosition(
        TLorentzVector(
            track->GetPosition().x() / CLHEP::cm,
            track->GetPosition().y() / CLHEP::cm,
            track->GetPosition().z() / CLHEP::cm,
            track->GetGlobalTime()   / CLHEP::ns
        )
    );

    particle->SetInitialMomentum(
        TLorentzVector(
            track->GetMomentum().x() / CLHEP::MeV,
            track->GetMomentum().y() / CLHEP::MeV,
            track->GetMomentum().z() / CLHEP::MeV,
            track->GetTotalEnergy()  / CLHEP::MeV
        )
    );

    if (track->GetParentID() > 0) {
        Particle* parent_particle = truth_manager->GetParticle(track->GetParentID());
        parent_particle->AddDaughter(track->GetTrackID());
    }

    truth_manager->AddParticle(particle);
}
//_________________________________________________________________________________________
void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    TruthManager* truth_manager = TruthManager::GetInstance();
    Particle* particle = truth_manager->GetParticle(track->GetTrackID());
}