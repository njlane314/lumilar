#ifndef ENERGY_DEPOSIT_HH
#define ENERGY_DEPOSIT_HH

#include <Eigen/Dense>
#include <string>

class EnergyDeposit {
public:
    EnergyDeposit();
    EnergyDeposit(double visible, double linear_transfer, const std::string particle_species, const Eigen::Vector3d position, double length, double time);
    ~EnergyDeposit();

    double getVisibleEnergy() const;
    void setVisibleEnergy(double value);

    double getLinearTransfer() const;
    void setLinearTransfer(double value);

    std::string getParticleSpecies() const;
    void setParticleSpecies(const std::string value);

    Eigen::Vector3d getPosition() const;
    void setPosition(const Eigen::Vector3d value);

    double getLength() const;
    void setLength(double value);

    double getTime() const;
    void setTime(double value);

    bool isEmpty() const;

private:
    double visible;
    double linear_transfer;
    std::string particle_species;
    Eigen::Vector3d position;
    double length;
    double time;
};

#endif // ENERGY_DEPOSIT_HH