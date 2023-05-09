#include "Scintillation.hh"

Scintillation* Scintillation::instance_ = nullptr;

Scintillation::Scintillation()
: photon_radiants_(), relaxation_generator_(Relaxation()) {}

Scintillation::~Scintillation() {}

void Scintillation::AddRadiant(const EnergyDeposit* energy_deposit, int radiant_size) {
    PhotonRadiant current_radiant = {};

    current_radiant.position = energy_deposit->GetPosition();
    for (int i = 0; i < radiant_size; i++) {
        OpticalPhoton photon = Relaxation::CreateOpticalPhoton(energy_deposit);
        if (photon.GetEmissionTime() > 0 && photon.GetWavelength() > 0) {
            current_radiant.photons.push_back(photon);
        }
    }
    photon_radiants_.push_back(current_radiant);
}

std::vector<PhotonRadiant> Scintillation::GetPhotonRadiants() const {
    return photon_radiants_;
}

std::vector<Eigen::Vector3d> Scintillation::GetRadiantPositions() const {
    std::vector<Eigen::Vector3d> radiant_positions;
    for (const auto& a_radiant : photon_radiants_) {
        radiant_positions.push_back(a_radiant.position);
    }
    return radiant_positions;
}

std::vector<int> Scintillation::GetRadiantSizes() const {
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

int Scintillation::GetTotalPhotonCount() const {
    int photon_count = 0;
    for (const auto& a_radiant : photon_radiants_) {
        for (const auto& photon : a_radiant.photons) {
            photon_count += 1;
        }
    }
    return photon_count;
}

std::vector<double> Scintillation::GetEmissionTimes() const {
    std::vector<double> emission_times;
    for (const auto& a_radiant : photon_radiants_) {
        for (const auto& photon : a_radiant.photons) {
            emission_times.push_back(photon.GetEmissionTime());
        }
    }
    return emission_times;
}

std::vector<double> Scintillation::GetWavelengths() const {
    std::vector<double> wavelengths;
    for (const auto& a_radiant : photon_radiants_) {
        for (const auto& photon : a_radiant.photons) {
            wavelengths.push_back(photon.GetWavelength());
        }
    }
    return wavelengths;
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
            std::cout << photon.GetEmissionTime() << " ";
        }
        std::cout << std::endl;
    }
}