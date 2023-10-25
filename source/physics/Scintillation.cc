#include "Scintillation.hh"
//_________________________________________________________________________________________
Scintillation* Scintillation::instance_ = nullptr;
//_________________________________________________________________________________________
Scintillation::Scintillation()
: photon_radiant_vector_() {}
//_________________________________________________________________________________________
Scintillation::~Scintillation() {}
//_________________________________________________________________________________________
void Scintillation::CreateRadiant(const EnergyDeposit* energy_deposit, int optical_photon_count) {
    std::string particle_name = energy_deposit->GetParticleSpecies();
    Eigen::Vector3d radiant_position = energy_deposit->GetPosition();
    double radiant_time = energy_deposit->GetTime();
    int radiant_photon_count = optical_photon_count;

    photon_radiant_vector_.push_back(PhotonRadiant(radiant_photon_count, radiant_time, radiant_position, particle_name));
}
//_________________________________________________________________________________________
std::vector<PhotonRadiant> Scintillation::GetPhotonRadiants() const {
    return photon_radiant_vector_;
}
//_________________________________________________________________________________________
std::vector<Eigen::Vector3d> Scintillation::GetRadiantPositions() const {
    std::vector<Eigen::Vector3d> radiant_positions;
    for (const auto& photon_radiant : photon_radiant_vector_) {
        radiant_positions.push_back(photon_radiant.GetPosition());
    }

    return radiant_positions;
}
//_________________________________________________________________________________________
std::vector<int> Scintillation::GetRadiantCounts() const {
    std::vector<int> radiant_count_vector;
    for (const auto& photon_radiant : photon_radiant_vector_) {
        radiant_count_vector.push_back(photon_radiant.GetPhotonCount());
    }

    return radiant_count_vector;
}
//_________________________________________________________________________________________
int Scintillation::GetTotalPhotonCount() const {
    int photon_count = 0;
    for (const auto& photon_radiant : photon_radiant_vector_) {
        photon_count += photon_radiant.GetPhotonCount();
    }
    
    return photon_count;
}