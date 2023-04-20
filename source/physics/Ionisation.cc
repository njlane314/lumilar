#include "Ionisation.hh"

Ionisation* Ionisation::instance_ = nullptr;

Ionisation::Ionisation() 
: ionisation_(), ejection_generator_(Ejection()) {}

Ionisation::~Ionisation() {}

void Ionisation::add_cloud(const EnergyDeposit* energy_deposit, int cloud_size) {
    ElectronCloud current_cloud = {};

    current_cloud.position = energy_deposit->get_position();
    for (int i = 0; i < cloud_size; i++) {
        current_cloud.electrons.push_back(ejection_generator_.create_electron());
    }

    ionisation_.push_back(current_cloud);
}

std::vector<int> Ionisation::get_cloud_sizes() const {
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

int Ionisation::get_total_electrons() const {
    int total_electrons = 0;
    for (const auto& a_cloud : ionisation_) {
        for (const auto& electron : a_cloud.electrons) {
            total_electrons += 1;
        }
    }
    return total_electrons;
}


std::vector<Eigen::Vector3d> Ionisation::get_cloud_positions() const {
    std::vector<Eigen::Vector3d> cloud_positions;
    for (const auto& a_cloud : ionisation_) {
        cloud_positions.push_back(a_cloud.position);
    }
    return cloud_positions;
}

void Ionisation::print_ionisation() const {
    std::cout << "ionisation clouds:" << std::endl;
    for (const auto& a_cloud : ionisation_) {
        std::cout << "  Position: (";
        for (const auto& p : a_cloud.position) {
            std::cout << p << ", ";
        }
        std::cout << "), electrons: " << a_cloud.electrons.size() << std::endl;
    }
}