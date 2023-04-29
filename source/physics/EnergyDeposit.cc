#include "EnergyDeposit.hh"

EnergyDeposit::EnergyDeposit() : visible(0), linear_transfer(0), particle_species(""), position(Eigen::Vector3d::Zero()), length(0), time(0) {}

EnergyDeposit::EnergyDeposit(double visible, double linear_transfer, std::string particle_species, Eigen::Vector3d position, double length, double time)
    : visible(visible), linear_transfer(linear_transfer), particle_species(particle_species), position(position), length(length), time(time) {}

EnergyDeposit::~EnergyDeposit() {}

double EnergyDeposit::getVisibleEnergy() const {
    return visible;
}

void EnergyDeposit::setVisibleEnergy(double value) {
    visible = value;
}

double EnergyDeposit::getLinearTransfer() const {
    return linear_transfer;
}

void EnergyDeposit::setLinearTransfer(double value) {
    linear_transfer = value;
}

std::string EnergyDeposit::getParticleSpecies() const {
    return particle_species;
}

void EnergyDeposit::setParticleSpecies(std::string value) {
    particle_species = value;
}

Eigen::Vector3d EnergyDeposit::getPosition() const {
    return position;
}

void EnergyDeposit::setPosition(Eigen::Vector3d value) {
    position = value;
}

double EnergyDeposit::getLength() const {
    return length;
}

void EnergyDeposit::setLength(double value) {
    length = value;
}

double EnergyDeposit::getTime() const {
    return time;
}

void EnergyDeposit::setTime(double value) {
    time = value;
}

bool EnergyDeposit::isEmpty() const {
    return visible == 0 || linear_transfer == 0 || particle_species.empty() || position.isZero() || length == 0 || time == 0;
}