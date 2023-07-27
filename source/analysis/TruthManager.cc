#include "TruthManager.hh"
//_________________________________________________________________________________________
TruthManager* TruthManager::instance_ = nullptr;
//_________________________________________________________________________________________
TruthManager::TruthManager() {}
//_________________________________________________________________________________________
TruthManager::~TruthManager() {}
//_________________________________________________________________________________________
TruthManager * TruthManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new TruthManager();
    }
    return instance_;
}
//_________________________________________________________________________________________
void TruthManager::EventReset() {
    initial_generator_particles_.clear();
    final_generator_particles_.clear();

    initial_marley_particles_.clear();
    final_marley_particles_.clear();

    for (auto p : particle_map_) {
        delete p.second;
    }

    particle_map_.clear();
}
//_________________________________________________________________________________________
void TruthManager::SetRun(const int value) {
    run_ = value;
}
//_________________________________________________________________________________________
void TruthManager::SetEvent(const int value) {
    event_ = value;
}
//_________________________________________________________________________________________
void TruthManager::AddInitialGeneratorParticle(GeneratorParticle* particle) {
    initial_generator_particles_.push_back(particle);
}
//_________________________________________________________________________________________
void TruthManager::AddFinalGeneratorParticle(GeneratorParticle* particle) {
    final_generator_particles_.push_back(particle);
}
//_________________________________________________________________________________________
void TruthManager::AddInitialMarleyParticle(const marley::Particle& particle) {
    initial_marley_particles_.push_back(particle);
}
//_________________________________________________________________________________________
void TruthManager::AddFinalMarleyParticle(const marley::Particle& particle) {
    final_marley_particles_.push_back(particle);
}
//_________________________________________________________________________________________
void TruthManager::AddParticle(Particle* particle) {
    particle_map_[particle->TrackID()] = particle;
}
//_________________________________________________________________________________________
Particle* TruthManager::GetParticle(const int trackID) {
    if (particle_map_.count(trackID) < 1) {
        std::string message = "\nLine "
                            + std::to_string(__LINE__)
                            + " of file "
                            + __FILE__
                            + "\n\nTrack ID not found: "
                            + std::to_string(trackID)
                            + "\n";
        G4Exception("TruthManager::TruthManager", "Error",
                    FatalException, message.data());
    }
    return particle_map_.at(trackID);
}