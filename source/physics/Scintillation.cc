#include "Scintillation.hh"

Scintillation* Scintillation::instance_ = nullptr;

Scintillation::Scintillation()
: photon_radiants_(), relaxation_generator_(Relaxation()) {}

Scintillation::~Scintillation() {}

void Scintillation::AddRadiant(const EnergyDeposit* energy_deposit, int radiant_size, double singlet_to_triplet) {
    PhotonRadiant current_radiant = {};

    current_radiant.position = energy_deposit->getPosition();
    for (int i = 0; i < radiant_size; i++) {
        OpticalPhoton photon = relaxation_generator_.CreateOpticalPhoton(energy_deposit, singlet_to_triplet);
        if (photon.getEmissionTime() != 0) {
            current_radiant.photons.push_back(photon);
        }
    }
    photon_radiants_.push_back(current_radiant);
}

std::vector<PhotonRadiant> Scintillation::getPhotonRadiants() const {
    return photon_radiants_;
}

std::vector<Eigen::Vector3d> Scintillation::getRadiantPositions() const {
    std::vector<Eigen::Vector3d> radiant_positions;
    for (const auto& a_radiant : photon_radiants_) {
        radiant_positions.push_back(a_radiant.position);
    }
    return radiant_positions;
}

std::vector<int> Scintillation::getRadiantSizes() const {
    std::vector<int> radiant_sizes;
    for (const auto& a_radiant : photon_radiants_) {
        int photon_count = 0;
        for (const auto& photon : a_radiant.photons) {
            photon_count += 1;
        }
        radiant_sizes.push_back(photon_count);
    }
    return radiant_sizes;
}

int Scintillation::getTotalPhotonCount() const {
    int photon_count = 0;
    for (const auto& a_radiant : photon_radiants_) {
        for (const auto& photon : a_radiant.photons) {
            photon_count += 1;
        }
    }
    return photon_count;
}

std::vector<double> Scintillation::getEmissionTimes() const {
    std::vector<double> emission_times;
    for (const auto& a_radiant : photon_radiants_) {
        for (const auto& photon : a_radiant.photons) {
            emission_times.push_back(photon.getEmissionTime());
        }
    }
    return emission_times;
}

void Scintillation::PrintScintillation() const {
    std::cout << "Scintillation radiants:" << std::endl;
    for (const auto& a_radiant : photon_radiants_) {
        std::cout << "  Position: (";
        for (const auto& p : a_radiant.position) {
            std::cout << p << ", ";
        }
        int photon_count = 0;
        for (const auto& photon : a_radiant.photons) {
            photon_count += 1;
        }
        std::cout << "), photons: " << photon_count << std::endl;

        std::cout << "  Emission times: ";
        for (const auto& photon : a_radiant.photons) {
            std::cout << photon.getEmissionTime() << " ";
        }
        std::cout << std::endl;
    }
}