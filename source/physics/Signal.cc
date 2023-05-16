#include "Signal.hh"
//_________________________________________________________________________________________
Signal* Signal::instance_ = nullptr;
//_________________________________________________________________________________________
Signal::Signal()
: medium_properties_(MediumProperties::GetInstance()->GetMediumProperties()) {
    if (!instance_) {
        instance_ = this;
    }

    if (!medium_properties_) {
        throw std::runtime_error("-- MediumProperties instance is null");
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
std::vector<double> Signal::GetVisibleDeposits() const {
    std::vector<double> visible_deposits;
    for (const auto& a_deposit : *track_structure_) {
        visible_deposits.push_back(a_deposit.GetVisibleEnergy());
    }
    return visible_deposits;
}
//_________________________________________________________________________________________
std::vector<double> Signal::GetLinearTransfers() const {
    std::vector<double> linear_transfers;
    for (const auto& a_deposit : *track_structure_) {
        linear_transfers.push_back(a_deposit.GetLinearTransfer());
    }
    return linear_transfers;
}
//_________________________________________________________________________________________
std::vector<double> Signal::GetLengths() const {
    std::vector<double> lengths;
    for (const auto& a_deposit : *track_structure_) {
        lengths.push_back(a_deposit.GetLength());
    }
    return lengths;
}
//_________________________________________________________________________________________
void Signal::RecordDelayTime(const double delay_time) {
    delay_times_.push_back(delay_time);
}
//_________________________________________________________________________________________
std::vector<double> Signal::GetDelayTimes() const {
    return delay_times_;
}
//_________________________________________________________________________________________
void Signal::RecordPrimaryEnergy(const double primary_energy) {
    primary_energy_ = primary_energy;
}
//_________________________________________________________________________________________
double Signal::GetPrimaryEnergy() const {
    return primary_energy_;
}
//_________________________________________________________________________________________
void Signal::DeleteSignal() {
    instance_ = nullptr;
    delete this;
}