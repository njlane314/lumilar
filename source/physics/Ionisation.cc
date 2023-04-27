#include "Ionisation.hh"

Ionisation* Ionisation::instance_ = nullptr;

Ionisation::Ionisation() 
: ionisation_(), ejection_generator_(Ejection()) {}

Ionisation::~Ionisation() {}

void Ionisation::AddCloud(const EnergyDeposit* energy_deposit, int cloud_size) {
    ElectronCloud current_cloud = {};

    current_cloud.position = energy_deposit->getPosition();
    for (int i = 0; i < cloud_size; i++) {
        current_cloud.electrons.push_back(ejection_generator_.CreateThermalElectron());
    }
    ionisation_.push_back(current_cloud);
}

std::vector<int> Ionisation::getCloudSizes() const {
    std::vector<int> cloud_sizes;
    for (const auto& a_cloud : ionisation_) {
        int electron_count = 0;
        for (const auto& electron : a_cloud.electrons) {
            electron_count += 1;
        }
        cloud_sizes.push_back(electron_count);
    }
    return cloud_sizes;
}

int Ionisation::getTotalElectronCount() const {
    int electron_count = 0;
    for (const auto& a_cloud : ionisation_) {
        for (const auto& electron : a_cloud.electrons) {
            electron_count += 1;
        }
    }
    return electron_count;
}

std::vector<Eigen::Vector3d> Ionisation::getCloudPositions() const {
    std::vector<Eigen::Vector3d> cloud_positions;
    for (const auto& a_cloud : ionisation_) {
        cloud_positions.push_back(a_cloud.position);
    }
    return cloud_positions;
}

void Ionisation::PrintIonisation() const {
    std::cout << "ionisation clouds:" << std::endl;
    for (const auto& a_cloud : ionisation_) {
        std::cout << "  Position: (";
        for (const auto& p : a_cloud.position) {
            std::cout << p << ", ";
        }
        int electron_count = 0;
        for (const auto& electron : a_cloud.electrons) {
            electron_count += 1;
        }
        std::cout << "), electrons: " << electron_count << std::endl;
    }
}