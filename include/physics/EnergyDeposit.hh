#ifndef ENERGY_DEPOSIT_HH
#define ENERGY_DEPOSIT_HH

#include <Eigen/Dense>
#include <string>

class EnergyDeposit {
public:
    EnergyDeposit();
    EnergyDeposit(double visible, double linear_transfer, const std::string particle_species, const Eigen::Vector3d position, double length, double time);
    ~EnergyDeposit();

    double GetVisibleEnergy() const;
    void SetVisibleEnergy(double value);

    double GetLinearTransfer() const;
    void SetLinearTransfer(double value);

    std::string GetParticleSpecies() const;
    void SetParticleSpecies(const std::string value);

    Eigen::Vector3d GetPosition() const;
    void SetPosition(const Eigen::Vector3d value);

    double GetLength() const;
    void SetLength(double value);

    double GetTime() const;
    void SetTime(double value);

    bool IsEmpty() const;

private:
    double visible;
    double linear_transfer;
    std::string particle_species;
    Eigen::Vector3d position;
    double length;
    double time;
};

#endif // ENERGY_DEPOSIT_HH