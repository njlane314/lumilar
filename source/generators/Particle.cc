#include "Particle.hh"
//_________________________________________________________________________________________
#include "G4VProcess.hh"
//_________________________________________________________________________________________
Particle::Particle() {}
//_________________________________________________________________________________________
Particle::~Particle() {}
//_________________________________________________________________________________________
void Particle::AddTrajectoryHit(const TrajectoryHit & hit) {
    hits_.push_back(hit);
}
//_________________________________________________________________________________________
void Particle::AddTrajectoryHit(const G4Step * step) {
    TrajectoryHit hit;
    hit.start_ = { step->GetPreStepPoint()->GetPosition().x()/CLHEP::cm,
                   step->GetPreStepPoint()->GetPosition().y()/CLHEP::cm,
                   step->GetPreStepPoint()->GetPosition().z()/CLHEP::cm, };
    hit.end_   = { step->GetPostStepPoint()->GetPosition().x()/CLHEP::cm,
                   step->GetPostStepPoint()->GetPosition().y()/CLHEP::cm,
                   step->GetPostStepPoint()->GetPosition().z()/CLHEP::cm, };
    hit.energy_deposit_ = step->GetTotalEnergyDeposit() / CLHEP::MeV;
    hit.start_time_ = step->GetPreStepPoint()->GetGlobalTime() / CLHEP::ns;
    hit.end_time_   = step->GetPostStepPoint()->GetGlobalTime() / CLHEP::ns;
    hit.track_id_   = step->GetTrack()->GetTrackID();
    hit.pdg_code_   = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
    hit.length_     = step->GetStepLength() / CLHEP::cm;
    hit.process_    = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

    energy_deposited_ += step->GetTotalEnergyDeposit() / CLHEP::MeV;

    this->AddTrajectoryHit(hit);
}
//_________________________________________________________________________________________
void Particle::AddDaughter(int const track_id) {
    daughter_track_ids_.push_back(track_id);
    number_daughters_++;
}