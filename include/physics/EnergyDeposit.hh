#ifndef ENERGY_DEPOSIT_HH
#define ENERGY_DEPOSIT_HH

#include <string>
#include <Eigen/Dense>

#include "LArParameters.h"

class EnergyDeposit {
public:
    EnergyDeposit(double visible_deposit, double stopping_power, const std::string& particle_species, const Eigen::Vector3d& position, double discrete_length, double time)
    : visible_deposit_(visible_deposit), stopping_power_(stopping_power), particle_species_(particle_species), position_(position), discrete_length_(discrete_length), time_(time) {}

    EnergyDeposit() : visible_deposit_(0), stopping_power_(0), particle_species_(""), position_(Eigen::Vector3d(0, 0, 0)), discrete_length_(0), time_(0) {}

    double GetVisibleDeposit() const { return visible_deposit_; }
    double GetStoppingPower() const { return stopping_power_; }
    const std::string& GetParticleSpecies() const { return particle_species_; }
    const Eigen::Vector3d& GetPosition() const { return position_; }
    double GetDiscreteLength() const { return discrete_length_; }
    double GetTime() const { return time_; }
    bool isEmpty() const { return energy_ <= 0 || dx_ <= 0 || time_ <= 0; };

private:
    double visible_deposit_;
    double stopping_power_;
    std::string particle_species_;
    Eigen::Vector3d position_;
    double discrete_length_;
    double time_;
};

    

#endif // ENERGY_DEPOSIT_HH