#include "EnergyDeposit.hh"

EnergyDeposit::EnergyDeposit() : visible(0), linear_transfer(0), particle_species(""), position(Eigen::Vector3d::Zero()), length(0), time(0) {}

EnergyDeposit::EnergyDeposit(double visible, double linear_transfer, std::string particle_species, Eigen::Vector3d position, double length, double time)
    : visible(visible), linear_transfer(linear_transfer), particle_species(particle_species), position(position), length(length), time(time) {}

EnergyDeposit::~EnergyDeposit() {}

double EnergyDeposit::GetVisibleEnergy() const { return visible; }

void EnergyDeposit::SetVisibleEnergy(double value) { visible = value; }

double EnergyDeposit::GetLinearTransfer() const { return linear_transfer; }

void EnergyDeposit::SetLinearTransfer(double value) { linear_transfer = value; }

std::string EnergyDeposit::GetParticleSpecies() const { return particle_species; }

void EnergyDeposit::SetParticleSpecies(std::string value) { particle_species = value; }

Eigen::Vector3d EnergyDeposit::GetPosition() const { return position; }

void EnergyDeposit::SetPosition(Eigen::Vector3d value) { position = value; }

double EnergyDeposit::GetLength() const { return length; }

void EnergyDeposit::SetLength(double value) { length = value; }

double EnergyDeposit::GetTime() const { return time; }

void EnergyDeposit::SetTime(double value) { time = value; }

bool EnergyDeposit::IsEmpty() const {
    return visible == 0 || linear_transfer == 0 || particle_species.empty() || position.isZero() || length == 0 || time == 0;
}