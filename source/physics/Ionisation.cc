#include "Ionisation.hh"
//_________________________________________________________________________________________
Ionisation* Ionisation::instance_ = nullptr;
//_________________________________________________________________________________________
Ionisation::Ionisation() 
: electron_cloud_vector_() {}
//_________________________________________________________________________________________
Ionisation::~Ionisation() {}
//_________________________________________________________________________________________
void Ionisation::CreateCloud(const EnergyDeposit* energy_deposit, int thermal_electron_count) {
    larnest::LArInteraction cloud_interaction = energy_deposit->GetInteractionSpecies();
    Eigen::Vector3d cloud_position = energy_deposit->GetPosition();
    double cloud_time = energy_deposit->GetTime();
    int cloud_electron_count = thermal_electron_count;

    electron_cloud_vector_.push_back(ElectronCloud(cloud_electron_count, cloud_time, cloud_position, cloud_interaction));
}
//_________________________________________________________________________________________
std::vector<ElectronCloud> Ionisation::GetElectronClouds() const {
    return electron_cloud_vector_;
}
//_________________________________________________________________________________________
std::vector<Eigen::Vector3d> Ionisation::GetCloudPositions() const {
    std::vector<Eigen::Vector3d> cloud_positions;
    for (const auto& electron_cloud : electron_cloud_vector_) {
        cloud_positions.push_back(electron_cloud.GetPosition());
    }
    
    return cloud_positions;
}
//_________________________________________________________________________________________
std::vector<int> Ionisation::GetCloudCounts() const {
    std::vector<int> cloud_count_vector;
    for (const auto& electron_cloud : electron_cloud_vector_) {
        cloud_count_vector.push_back(electron_cloud.GetElectronCount());
    }

    return cloud_count_vector;
}
//_________________________________________________________________________________________
int Ionisation::GetTotalElectronCount() const {
    int electron_count = 0;
    for (const auto& electron_cloud : electron_cloud_vector_) {
        electron_count += electron_cloud.GetElectronCount();
    }

    return electron_count;
}