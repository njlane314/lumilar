#include "EnergyDeposit.hh"
//_________________________________________________________________________________________
EnergyDeposit::EnergyDeposit() : visible(0), linear_transfer(0), particle_species(""), position(Eigen::Vector3d::Zero()), length(0), time(0) {}
//_________________________________________________________________________________________
EnergyDeposit::EnergyDeposit(double visible, double linear_transfer, std::string particle_species, Eigen::Vector3d position, double length, double time)
    : visible(visible), linear_transfer(linear_transfer), particle_species(particle_species), position(position), length(length), time(time) {}
//_________________________________________________________________________________________
EnergyDeposit::~EnergyDeposit() {}
//_________________________________________________________________________________________
double EnergyDeposit::GetVisibleEnergy() const { 
    return visible; 
}
//_________________________________________________________________________________________
void EnergyDeposit::SetVisibleEnergy(double value) { 
    visible = value; 
}//_________________________________________________________________________________________

double EnergyDeposit::GetLinearTransfer() const { 
    return linear_transfer; 
}
//_________________________________________________________________________________________
void EnergyDeposit::SetLinearTransfer(double value) { 
    linear_transfer = value; 
}
//_________________________________________________________________________________________
std::string EnergyDeposit::GetParticleSpecies() const { 
    return particle_species; 
}
//_________________________________________________________________________________________
void EnergyDeposit::SetParticleSpecies(std::string value) { 
    particle_species = value; 
}
//_________________________________________________________________________________________
Eigen::Vector3d EnergyDeposit::GetPosition() const { 
    return position; 
}
//_________________________________________________________________________________________
void EnergyDeposit::SetPosition(Eigen::Vector3d value) { 
    position = value; 
}
//_________________________________________________________________________________________
double EnergyDeposit::GetLength() const { 
    return length; 
}
//_________________________________________________________________________________________
void EnergyDeposit::SetLength(double value) { 
    length = value; 
}
//_________________________________________________________________________________________
double EnergyDeposit::GetTime() const { 
    return time; 
}
//_________________________________________________________________________________________
void EnergyDeposit::SetTime(double value) { 
    time = value; 
}
//_________________________________________________________________________________________
bool EnergyDeposit::IsEmpty() const {
    return visible == 0 || linear_transfer == 0 || particle_species.empty() || position.isZero() || length == 0 || time == 0;
}