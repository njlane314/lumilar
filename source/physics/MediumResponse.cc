#include "MediumResponse.hh"

MediumResponse::MediumResponse() {}

MediumResponse::~MediumResponse() {}

EnergyDeposit* MediumResponse::CreateEnergyDeposit(const G4Step* step) {
    const int particle_charge = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetPDGCharge();
    const int entering_material = static_cast<int>(step->GetPreStepPoint()->GetMaterial()->GetZ());
    const double intrinsic_threshold = MaterialProperties::getInstance()->getMaterialProperties()->loss_per_ionisation;
    const double visible_deposit = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();
    if (particle_charge != 0 || entering_material == 18 || visible_deposit > intrinsic_threshold) {
        const double stopping_power = step->GetTotalEnergyDeposit() / step->GetStepLength();
        const std::string particle_species = step->GetTrack()->GetDefinition()->GetParticleName();
        const double discrete_length = step->GetStepLength();
        const Eigen::Vector3d position(step->GetPreStepPoint()->GetPosition().x(), step->GetPreStepPoint()->GetPosition().y(), step->GetPreStepPoint()->GetPosition().z());
        const double time = step->GetPreStepPoint()->GetGlobalTime();

        auto energy_deposit = new EnergyDeposit(visible_deposit, stopping_power, particle_species, position, discrete_length, time);
        Signal::getInstance()->addEnergyDeposit(energy_deposit);
        return energy_deposit;
    }

    return new EnergyDeposit();
}

void MediumResponse::ProcessResponse(const G4Step* step) {
    const auto energy_deposit = CreateEnergyDeposit(step);
    if (!energy_deposit->isEmpty()) {
        const std::string particle_name = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
        int num_thermal_electrons, num_optical_photons;
        double component_ratio;
        if (particle_name == "e-" || particle_name == "e+" || particle_name == "mu-" || particle_name == "mu+" || particle_name == "proton") /* i.e. light particle */ {
            component_ratio = MaterialProperties::getInstance()->getMaterialProperties()->singlet_to_triplet_light;
            const auto intrinsic_response = Excitation::CreateExctiation(energy_deposit, MaterialProperties::getInstance()->getMaterialProperties());
            std::tie(num_thermal_electrons, num_optical_photons) = Recombination::ProcessRecombination(energy_deposit, MaterialProperties::getInstance()->getMaterialProperties(), intrinsic_response);
        }
        else {
            return;
        }

        Signal::getInstance()->getScintillation()->AddRadiant(energy_deposit, num_optical_photons, component_ratio);
        Signal::getInstance()->getIonisation()->AddCloud(energy_deposit, num_thermal_electrons);
    }
}