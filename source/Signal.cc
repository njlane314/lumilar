#include "Signal.hh"

Signal* Signal::instance_ = nullptr;

Signal::Signal()
: material_properties_(MaterialProperties::get_instance()->get_material_properties()) {
    if (!instance_) {
        instance_ = this;
    }

    scintillation_ = new Scintillation();
}

Signal::~Signal() {}

Signal* Signal::get_instance() {
    if (!instance_) {
        instance_ = new Signal();
    }
    return instance_;
}

Scintillation* Signal::get_scintillation() {
    return scintillation_;
}

EnergyDeposit* Signal::create_energy_deposit(const G4Step* step) {
    EnergyDeposit* energy_deposit = new EnergyDeposit(); 
    G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
    energy_deposit->visible = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();
    energy_deposit->linear_transfer = (step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit()) / step->GetStepLength();
    energy_deposit->particle_type = step->GetTrack()->GetDefinition()->GetParticleName();
    energy_deposit->position = { pos.x(), pos.y(), pos.z() };
    energy_deposit->time = step->GetPreStepPoint()->GetGlobalTime();

    return energy_deposit;
}

void Signal::process_response(const G4Step* step) {
    EnergyDeposit* energy_deposit = create_energy_deposit(step);

    int particle_charge = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetPDGCharge();
	int entering_material = (int) step->GetPreStepPoint()->GetMaterial()->GetZ();

	std::string particle_name = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();

    double intrinsic_threshold = material_properties_->loss_per_ionisation;
    double deposit = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();

    int thermal_electrons_size, optical_photons_size;
	if (particle_charge != 0 && entering_material == 18 && deposit > intrinsic_threshold) {
        if (particle_name == "e-" || particle_name == "e+" || particle_name == "mu-" || particle_name == "mu+") /* i.e. light particle */ {
            MediumResponse medium_response;
            double singlet_to_triplet = material_properties_->singlet_to_triplet_light;

            std::tie(thermal_electrons_size, optical_photons_size) = medium_response.create_response(energy_deposit);
            scintillation_->add_radiant(optical_photons_size, energy_deposit->position, energy_deposit->time, singlet_to_triplet);
        }
    }
}