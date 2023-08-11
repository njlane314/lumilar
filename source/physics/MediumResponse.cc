#include "MediumResponse.hh"
//_________________________________________________________________________________________
MediumResponse::MediumResponse() {}
//_________________________________________________________________________________________
MediumResponse::~MediumResponse() {}
//_________________________________________________________________________________________
larnest::LArNESTResult MediumResponse::ProcessResponse(const G4Step* step) {
    larnest::LArNEST nest; 
    larnest::LArNESTResult result;

    const EnergyDeposit* energy_deposit = CreateEnergyDeposit(step);
    if (!energy_deposit->isEmpty() || energy_deposit->GetEnergy() > 0) {
        const std::string particle_name = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
        int thermal_electron_count, optical_photon_count;

        larnest::LArInteraction interaction = energy_deposit->GetInteractionSpecies();
        double energy = energy_deposit->GetEnergy();
        double dx = energy_deposit->GetStepLength();
        double electric_field = 0.5 * 1000.;
        double density = larnest::legacy_density_LAr;
        bool do_times = false;

        result = nest.FullCalculation(interaction, energy, dx, electric_field, density, do_times);

        bool debug_nest_results = false;
        if (debug_nest_results) {
            PrintResponse(result);
        }

        thermal_electron_count = (int)round(result.fluctuations.NeFluctuation);
        if (thermal_electron_count < 0) {
            thermal_electron_count = 0;
        }
        optical_photon_count = (int)round(result.fluctuations.NphFluctuation);
        if (optical_photon_count < 0) {
            optical_photon_count = 0;
        }

        result.fluctuations.NphFluctuation = optical_photon_count;
        result.fluctuations.NeFluctuation = thermal_electron_count;

        Signal::GetInstance()->GetScintillation()->CreateRadiant(energy_deposit, optical_photon_count);
        Signal::GetInstance()->GetIonisation()->CreateCloud(energy_deposit, thermal_electron_count);
    }

    return result;
}
//_________________________________________________________________________________________
EnergyDeposit* MediumResponse::CreateEnergyDeposit(const G4Step* step) {
    if (isStepInLiquidArgon(step) && isParticleCharged(step)) {
        const double energy = step->GetTotalEnergyDeposit() * 1000.;
        const double dx = step->GetStepLength() / 1000.;

        const std::string particle_name = step->GetTrack()->GetDefinition()->GetParticleName();
        
        larnest::LArInteraction interaction;

        if (
            particle_name == "e-" ||
            particle_name == "e+" ||
            particle_name == "mu-" ||
            particle_name == "mu+" ||
            particle_name == "proton" ||
            particle_name == "anti_proton"
        ) {
            interaction = larnest::LArInteraction::ER;
        } 

        if (
            particle_name == "alpha" 
        ) {
            interaction = larnest::LArInteraction::Alpha;
        }

        if (
            particle_name == "neutron"
        ) {
            interaction == larnest::LArInteraction::NR;
        }

        const Eigen::Vector3d position(step->GetPreStepPoint()->GetPosition().x(), step->GetPreStepPoint()->GetPosition().y(), step->GetPreStepPoint()->GetPosition().z());

        const double time = step->GetPreStepPoint()->GetGlobalTime();

        EnergyDeposit* energy_deposit = new EnergyDeposit(energy, dx, interaction, position, time);

        Signal::GetInstance()->AddEnergyDeposit(energy_deposit);

        return energy_deposit;
    }

    return new EnergyDeposit();
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
//_________________________________________________________________________________________
void MediumResponse::PrintResponse(const larnest::LArNESTResult& result) {
    std::string line = "=======================================================================";
    std::string subheader = "Liquid Argon Response NEST Results";
    std::cout << std::endl;
    std::cout << line << std::endl;
    std::cout << std::setw((line.length() - subheader.length()) / 2) << std::setfill(' ') << std::right << "" << subheader << std::setw((line.length() - subheader.length()) / 2) << std::setfill(' ') << std::left << std::endl;
    std::cout << line << std::endl;

    std::cout << std::setw(30) << std::left << "-------------Yields-------------" << std::endl;
    std::cout << std::setw(30) << std::left << "Total Yield: " << result.yields.TotalYield << std::endl;
    std::cout << std::setw(30) << std::left << "Quanta Yield: " << result.yields.QuantaYield << std::endl;
    std::cout << std::setw(30) << std::left << "Light Yield: " << result.yields.LightYield << std::endl;
    std::cout << std::setw(30) << std::left << "Nph: " << result.yields.Nph << std::endl;
    std::cout << std::setw(30) << std::left << "Ne: " << result.yields.Ne << std::endl;
    std::cout << std::setw(30) << std::left << "Nex: " << result.yields.Nex << std::endl;
    std::cout << std::setw(30) << std::left << "Nion: " << result.yields.Nion << std::endl;
    std::cout << std::setw(30) << std::left << "Lindhard: " << result.yields.Lindhard << std::endl;
    std::cout << std::setw(30) << std::left << "Electric Field: " << result.yields.ElectricField << std::endl;

    std::cout << std::setw(30) << std::left << "----------Fluctuations----------" << std::endl;
    std::cout << std::setw(30) << std::left << "Nph Fluctuation: " << result.fluctuations.NphFluctuation << std::endl;
    std::cout << std::setw(30) << std::left << "Ne Fluctuation: " << result.fluctuations.NeFluctuation << std::endl;
    std::cout << std::setw(30) << std::left << "Nex Fluctuation: " << result.fluctuations.NexFluctuation << std::endl;
    std::cout << std::setw(30) << std::left << "Nion Fluctuation: " << result.fluctuations.NionFluctuation << std::endl;

    std::cout << line << std::endl << std::endl;
}
