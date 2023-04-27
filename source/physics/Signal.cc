#include "Signal.hh"

Signal* Signal::instance_ = nullptr;

Signal::Signal()
: material_properties_(MaterialProperties::getInstance()->getMaterialProperties()) {
    if (!instance_) {
        instance_ = this;
    }

    if (!material_properties_) {
        throw std::runtime_error("-- MaterialProperties instance is null");
    }

    scintillation_ = std::make_unique<Scintillation>();
    ionisation_ = std::make_unique<Ionisation>();

    track_structure_ = std::make_unique<std::vector<EnergyDeposit>>();
}

Signal::~Signal() {
    delete instance_;
    instance_ = nullptr;
}

Signal* Signal::getInstance() {
    if (!instance_) {
        instance_ = new Signal();
    }
    return instance_;
}

Scintillation* Signal::getScintillation() const {
    return scintillation_.get();
}

Ionisation* Signal::getIonisation() const {
    return ionisation_.get();
}

EnergyDeposit* Signal::getCurrentEnergyDeposit() {
    return energy_deposit_.get();
}

void Signal::AddEnergyDeposit(const EnergyDeposit* energy_deposit) {
    track_structure_->push_back(*energy_deposit);
}

std::vector<double> Signal::getVisibleDeposits() const {
    std::vector<double> visible_deposits;
    for (const auto& a_deposit : *track_structure_) {
        visible_deposits.push_back(a_deposit.getVisibleEnergy());
    }
    return visible_deposits;
}

std::vector<double> Signal::getLinearTransfers() const {
    std::vector<double> linear_transfers;
    for (const auto& a_deposit : *track_structure_) {
        linear_transfers.push_back(a_deposit.getLinearTransfer());
    }
    return linear_transfers;
}

std::vector<double> Signal::getLengths() const {
    std::vector<double> lengths;
    for (const auto& a_deposit : *track_structure_) {
        lengths.push_back(a_deposit.getLength());
    }
    return lengths;
}

void Signal::RecordDelayTime(const double delay_time) {
    delay_times_.push_back(delay_time);
}

std::vector<double> Signal::getDelayTimes() const {
    return delay_times_;
}

void Signal::RecordPrimaryEnergy(const double primary_energy) {
    primary_energy_ = primary_energy;
}

double Signal::getPrimaryEnergy() const {
    return primary_energy_;
}

void Signal::DeleteSignal() {
    instance_ = nullptr;
    delete this;
}

// move to medium response
void Signal::CreateEnergyDeposit(const G4Step* step) {
    double visible = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();
    double linear_transfer = step->GetTotalEnergyDeposit() / step->GetStepLength();
    std::string particle_type = step->GetTrack()->GetDefinition()->GetParticleName();
    double length = step->GetStepLength();
    Eigen::Vector3d position(step->GetPreStepPoint()->GetPosition().x(), step->GetPreStepPoint()->GetPosition().y(), step->GetPreStepPoint()->GetPosition().z());
    double time = step->GetPreStepPoint()->GetGlobalTime();

    energy_deposit_ = std::make_unique<EnergyDeposit>(visible, linear_transfer, particle_type, position, length, time);
    AddEnergyDeposit(energy_deposit_.get());
}

void Signal::ProcessResponse(const G4Step* step) {
    CreateEnergyDeposit(step);
    EnergyDeposit* energy_deposit = getCurrentEnergyDeposit();

    int particle_charge = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetPDGCharge();
	int entering_material = (int) step->GetPreStepPoint()->GetMaterial()->GetZ();

	std::string particle_name = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();

    double intrinsic_threshold = material_properties_->loss_per_ionisation;
    double deposit = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();

    int thermal_electrons_size, optical_photons_size;
	if (particle_charge != 0 && entering_material == 18 && deposit > intrinsic_threshold) {
        if (particle_name == "e-" || particle_name == "e+" || particle_name == "mu-" || particle_name == "mu+" || particle_name == "proton") /* i.e. light particle */ {
            MediumResponse medium_response;
            double singlet_to_triplet = material_properties_->singlet_to_triplet_light;

            std::tie(thermal_electrons_size, optical_photons_size) = medium_response.CreateResponse(energy_deposit);
            scintillation_->AddRadiant(energy_deposit, optical_photons_size, singlet_to_triplet);
            ionisation_->AddCloud(energy_deposit, thermal_electrons_size);
        }
    }
}