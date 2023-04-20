#include "Scintillation.hh"

Scintillation* Scintillation::instance_ = nullptr;

Scintillation::Scintillation()
: scintillation_(), relaxation_generator_(Relaxation()) {}

Scintillation::~Scintillation() {}

void Scintillation::add_radiant(const EnergyDeposit* energy_deposit, int radiant_size, double singlet_to_triplet) {
    PhotonRadiant current_radiant = {};

    current_radiant.position = energy_deposit->get_position();
    for (int i = 0; i < radiant_size; i++) {
        OpticalPhoton photon = relaxation_generator_.create_photon(energy_deposit, singlet_to_triplet);
        if (photon.get_emission_time() != 0) {
            current_radiant.photons.push_back(photon);
        }
    }
    
    scintillation_.push_back(current_radiant);
}

std::vector<PhotonRadiant> Scintillation::get_photon_radiants() const {
    return scintillation_;
}

std::vector<Eigen::Vector3d> Scintillation::get_radiant_positions() const {
    std::vector<Eigen::Vector3d> radiant_positions;
    for (const auto& a_radiant : scintillation_) {
        radiant_positions.push_back(a_radiant.position);
    }
    return radiant_positions;
}

std::vector<int> Scintillation::get_radiant_sizes() const {
    std::vector<int> radiant_sizes;
    for (const auto& a_radiant : scintillation_) {
        int photon_count = 0;
        for (const auto& photon : a_radiant.photons) {
            photon_count += 1;
        }
        radiant_sizes.push_back(photon_count);
    }
    return radiant_sizes;
}

int Scintillation::get_total_photons() const {
    int total_photons = 0;
    for (const auto& a_radiant : scintillation_) {
        for (const auto& photon : a_radiant.photons) {
            total_photons += 1;
        }
    }
    return total_photons;
}

std::vector<double> Scintillation::get_emission_times() const {
    std::vector<double> emission_times;
    for (const auto& a_radiant : scintillation_) {
        for (const auto& photon : a_radiant.photons) {
            emission_times.push_back(photon.get_emission_time());
        }
    }
    return emission_times;
}

void Scintillation::print_scintillation() const {
    std::cout << "Scintillation radiants:" << std::endl;
    for (const auto& a_radiant : scintillation_) {
        std::cout << "  Position: (";
        for (const auto& p : a_radiant.position) {
            std::cout << p << ", ";
        }
        std::cout << "), photons: " << a_radiant.photons.size() << std::endl;

        std::cout << "  Emission times: ";
        for (const auto& photon : a_radiant.photons) {
            std::cout << photon.get_emission_time() << " ";
        }
        std::cout << std::endl;
    }
}