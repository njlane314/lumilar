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
        energy_deposits.push_back(energy_deposit.GetEnergy());
    }
    return energy_deposits;
}
//_________________________________________________________________________________________
std::vector<double> Signal::GetLinearEnergyTransfers() const {
    std::vector<double> linear_energy_transfers;
    for (const auto& energy_deposit : *track_structure_) {
        linear_energy_transfers.push_back(energy_deposit.GetEnergy() / energy_deposit.GetStepLength());
    }
    return linear_energy_transfers;
}
//_________________________________________________________________________________________
std::vector<double> Signal::GetLengths() const {
    std::vector<double> lengths;
    for (const auto& energy_deposit : *track_structure_) {
        lengths.push_back(energy_deposit.GetStepLength());
    }
    return lengths;
}
///////
void Signal::AddPrimaryEnergy(double primary_energy) {
    primary_energy_->push_back(primary_energy);
}
///////
std::vector<double>* Signal::GetPrimaryEnergy() const {
    return primary_energy_.get();
}
//_________________________________________________________________________________________
void Signal::DeleteSignal() {
    instance_ = nullptr;
    delete this;
}
