#include "Scintillation.hh"

Scintillation* Scintillation::instance_ = nullptr;

Scintillation::Scintillation()
: energy_deposits_(), scintillation_(), relaxation_generator_(Relaxation()) {}

Scintillation::~Scintillation() {}

void Scintillation::add_radiant(const EnergyDeposit* energy_deposit, int radiant_size, const std::vector<double>& position, double time, double singlet_to_triplet) {
    PhotonRadiant current_radiant = {};

    current_radiant.position = position;
    for (int i = 0; i < radiant_size; i++) {
        current_radiant.photons.push_back(relaxation_generator_.create_photon(time, singlet_to_triplet));
    }
    
    scintillation_.push_back(current_radiant);
    energy_deposits_.push_back(energy_deposit);
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

std::vector<std::vector<double>> Scintillation::get_radiant_positions() const {
    std::vector<std::vector<double>> radiant_positions;
    for (const auto& a_radiant : scintillation_) {
        radiant_positions.push_back(a_radiant.position);
    }
    return radiant_positions;
}

std::vector<const EnergyDeposit*> Scintillation::get_energy_deposits() const {
    return energy_deposits_;
}

std::vector<double> Scintillation::get_visible_deposits() const {
    std::vector<double> visible_deposits;
    for (const auto&energy_deposit : energy_deposits_) {
        visible_deposits.push_back(energy_deposit->visible);
    }
    return visible_deposits;
}

std::vector<double> Scintillation::get_radiant_sizes() const {
    std::vector<double> radiant_sizes;
    for (const auto& a_radiant : scintillation_) {
        radiant_sizes.push_back(a_radiant.photons.size());
    }
    return radiant_sizes;
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