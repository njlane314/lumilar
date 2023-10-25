#include "Signal.hh"
//_________________________________________________________________________________________
Signal* Signal::instance_ = nullptr;
//_________________________________________________________________________________________
Signal::Signal() {
    if (!instance_) {
        instance_ = this;
    }

    scintillation_ = std::make_unique<Scintillation>();
    ionisation_ = std::make_unique<Ionisation>();

    track_structure_ = std::make_unique<std::vector<EnergyDeposit>>();
}
//_________________________________________________________________________________________
Signal::~Signal() {
    delete instance_;
    instance_ = nullptr;
}
//_________________________________________________________________________________________
Signal* Signal::GetInstance() {
    if (!instance_) {
        instance_ = new Signal();
    }
    return instance_;
}
//_________________________________________________________________________________________
Scintillation* Signal::GetScintillation() const {
    return scintillation_.get();
}
//_________________________________________________________________________________________
Ionisation* Signal::GetIonisation() const {
    return ionisation_.get();
}
//_________________________________________________________________________________________
EnergyDeposit* Signal::GetCurrentEnergyDeposit() {
    return energy_deposit_.get();
}
//_________________________________________________________________________________________
void Signal::AddEnergyDeposit(const EnergyDeposit* energy_deposit) {
    track_structure_->push_back(*energy_deposit);
}
//_________________________________________________________________________________________
std::vector<EnergyDeposit>* Signal::GetHits() const {
    return track_structure_.get();
}
//_________________________________________________________________________________________
std::vector<double> Signal::GetEnergyDeposits() const {
    std::vector<double> energy_deposits;
    for (const auto& energy_deposit : *track_structure_) {
        energy_deposits.push_back(energy_deposit.GetVisibleDeposit());
    }
    return energy_deposits;
}
//_________________________________________________________________________________________
std::vector<double> Signal::GetLinearEnergyTransfers() const {
    std::vector<double> linear_energy_transfers;
    for (const auto& energy_deposit : *track_structure_) {
        linear_energy_transfers.push_back(energy_deposit.GetVisibleDeposit() / energy_deposit.GetDiscreteLength());
    }
    return linear_energy_transfers;
}
//_________________________________________________________________________________________
std::vector<double> Signal::GetLengths() const {
    std::vector<double> lengths;
    for (const auto& energy_deposit : *track_structure_) {
        lengths.push_back(energy_deposit.GetDiscreteLength());
    }
    return lengths;
}
//_________________________________________________________________________________________
marley::Event& Signal::GetMarleyEvent() {
    return marley_event_;
}
//_________________________________________________________________________________________
G4ThreeVector& Signal::GetMarleyInteractionVertex() {
    return marley_interaction_vertex_;
}
//_________________________________________________________________________________________
void Signal::SetMarleyEvent(const marley::Event& event) {
    marley_event_ = event;
}
//_________________________________________________________________________________________
void Signal::SetMarleyInteractionVertex(const G4ThreeVector& vertex) {
    marley_interaction_vertex_ = vertex;
}
//_________________________________________________________________________________________
void Signal::DeleteSignal() {
    instance_ = nullptr;
    delete this;
}
