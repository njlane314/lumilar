#ifndef ENERGY_DEPOSIT_HH
#define ENERGY_DEPOSIT_HH

#include <Eigen/Dense>

class EnergyDeposit {
public:
    EnergyDeposit(double visible, double linear_transfer, std::string particle_type, Eigen::Vector3d position, double length, double time) : visible(visible), linear_transfer(linear_transfer), particle_type(particle_type), position(position), length(length), time(time) {};
    ~EnergyDeposit() {};

    double getVisibleEnergy() const { return visible; }
    void setVisibleEnergy(double value) { visible = value; }
    
    double getLinearTransfer() const { return linear_transfer; }
    void setLinearTransfer(double value) { linear_transfer = value; }
    
    std::string getParticleType() const { return particle_type; }
    void setParticleType(std::string value) { particle_type = value; }
    
    Eigen::Vector3d getPosition() const { return position; }
    void setPosition(Eigen::Vector3d value) { position = value; }

    double getLength() const { return length; }
    void setLength(double value) { length = value; }
    
    double getTime() const { return time; }
    void setTime(double value) { time = value; }
    
private:
    double visible;
    double linear_transfer;
    std::string particle_type;
    Eigen::Vector3d position;
    double length;
    double time;
};

#endif // ENERGY_DEPOSIT_HH