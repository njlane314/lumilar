#include "MediumResponse.hh"
//_________________________________________________________________________________________
MediumResponse::MediumResponse() {}
//_________________________________________________________________________________________
MediumResponse::~MediumResponse() {}
//_________________________________________________________________________________________
EnergyDeposit* MediumResponse::CreateEnergyDeposit(const G4Step* step) {
    const int particle_charge = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetPDGCharge();
    const int entering_material = static_cast<int>(step->GetPreStepPoint()->GetMaterial()->GetZ());
    const double visible_deposit = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();
    
    if (isStepInLiquidArgon(step) && isParticleCharged(step)) {
        const double stopping_power = step->GetTotalEnergyDeposit() / step->GetStepLength();
        
        const std::string particle_species = step->GetTrack()->GetDefinition()->GetParticleName();
        const double discrete_length = step->GetStepLength();
        const Eigen::Vector3d position(step->GetPreStepPoint()->GetPosition().x(), step->GetPreStepPoint()->GetPosition().y(), step->GetPreStepPoint()->GetPosition().z());
        const double time = step->GetPreStepPoint()->GetGlobalTime();

        auto energy_deposit = new EnergyDeposit(visible_deposit, stopping_power, particle_species, position, discrete_length, time);
        Signal::GetInstance()->AddEnergyDeposit(energy_deposit);
        return energy_deposit;
    }

    return new EnergyDeposit();
}
//_________________________________________________________________________________________
void MediumResponse::ProcessResponse(const G4Step* step) {
    const auto energy_deposit = CreateEnergyDeposit(step);
    const auto properties =  MediumProperties::GetInstance()->GetMediumProperties()
    if (energy_deposit->GetEnergy() > 0) {
        const std::string particle_name = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
        int num_thermal_electrons, num_optical_photons;
        if (particle_name == "e-" || particle_name == "e+" || particle_name == "mu-" || particle_name == "mu+" || particle_name == "proton" || particle_name == "anti_proton" || particle_name == "kaon+" || particle_name == "kaon-" || ) {
            const auto intrinsic_response = Excitation::CreateExcitation(energy_deposit, properties);
            std::tie(num_thermal_electrons, num_optical_photons) = Recombination::ProcessRecombination(energy_deposit, properties, intrinsic_response);
        } else {
            return;
        }

        Signal::GetInstance()->GetScintillation()->CreateRadiant(energy_deposit, optical_photon_count);
        Signal::GetInstance()->GetIonisation()->CreateCloud(energy_deposit, thermal_electron_count);
    }
}
//_________________________________________________________________________________________
bool MediumResponse::isStepInLiquidArgon(const G4Step* step) {
    const int entering_material_atomic_number = static_cast<int>(step->GetPreStepPoint()->GetMaterial()->GetZ());

    if (entering_material_atomic_number == 18) {
        return true;
    }

    return false;
}
//_________________________________________________________________________________________
bool MediumResponse::isParticleCharged(const G4Step* step) {
    G4Track* track = step->GetTrack();
    const G4ParticleDefinition* particleDef = track->GetDefinition();

    double charge = particleDef->GetPDGCharge();

    if (charge != 0.0) {
        return true;
    } else {
        return false;
    }
}