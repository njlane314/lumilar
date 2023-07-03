#include "Scintillation.hh"
//_________________________________________________________________________________________
Scintillation* Scintillation::instance_ = nullptr;
//_________________________________________________________________________________________
Scintillation::Scintillation()
: photon_radiants_(), relaxation_generator_(Relaxation()) {}
//_________________________________________________________________________________________
Scintillation::~Scintillation() {}
//_________________________________________________________________________________________
void Scintillation::AddRadiant(const EnergyDeposit* energy_deposit, int num_optical_photons) {
    std::vector<OpticalPhoton> optical_photons;
    for (int i = 0; i < num_optical_photons; i++) {
        OpticalPhoton photon = Relaxation::CreateOpticalPhoton(energy_deposit);
        if (photon.GetEmissionTime() > 0 && photon.GetWavelength() > 0) {
            optical_photons.push_back(photon);
        }
    }

    photon_radiants_.push_back(PhotonRadiant(energy_deposit->GetPosition(), optical_photons));
}
//_________________________________________________________________________________________
std::vector<PhotonRadiant> Scintillation::GetPhotonRadiants() const {
    return photon_radiants_;
}
//_________________________________________________________________________________________
std::vector<Eigen::Vector3d> Scintillation::GetRadiantPositions() const {
    std::vector<Eigen::Vector3d> radiant_positions;
    for (const auto& photon_radiant : photon_radiants_) {
        radiant_positions.push_back(photon_radiant.GetPosition());
    }
    return radiant_positions;
}
//_________________________________________________________________________________________
std::vector<int> Scintillation::GetRadiantCounts() const {
    std::vector<int> radiant_counts;
    for (const auto& photon_radiant : photon_radiants_) {
        int photon_count = 0;
        for (const auto& photon : photon_radiant.GetOpticalPhotons()) {
            photon_count += 1;
        }
        radiant_counts.push_back(photon_count);
    }
    return radiant_counts;
}
//_________________________________________________________________________________________
int Scintillation::GetTotalPhotonCount() const {
    int photon_count = 0;
    for (const auto& photon_radiant : photon_radiants_) {
        for (const auto& photon : photon_radiant.GetOpticalPhotons()) {
            photon_count += 1;
        }
    }
    return photon_count;
}
//_________________________________________________________________________________________
std::vector<double> Scintillation::GetEmissionTimes() const {
    std::vector<double> emission_times;
    for (const auto& photon_radiant : photon_radiants_) {
        for (const auto& optical_photon : photon_radiant.GetOpticalPhotons()) {
            emission_times.push_back(optical_photon.GetEmissionTime());
        }
    }
    return emission_times;
}
//_________________________________________________________________________________________
std::vector<double> Scintillation::GetWavelengths() const {
    std::vector<double> wavelengths;
    for (const auto& photon_radiant: photon_radiants_) {
        for (const auto& optical_photon : photon_radiant.GetOpticalPhotons()) {
            wavelengths.push_back(optical_photon.GetWavelength());
        }
    }
    return wavelengths;
}
