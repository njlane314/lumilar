#ifndef ENERGY_DEPOSIT_HH
#define ENERGY_DEPOSIT_HH

#include <string>
#include <Eigen/Dense>

#include "LArParameters.h"

class EnergyDeposit {
public:
    EnergyDeposit() {
        energy_ = -1;
        dx_ = -1;
        species_ = larnest::LArInteraction::ER;
        position_ = Eigen::Vector3d::Zero();
        time_ = -1;
    };
    EnergyDeposit(double energy, double dx, larnest::LArInteraction species, Eigen::Vector3d position, double time) {
        energy_ = energy;
        dx_ = dx;
        species_ = species;
        position_ = position; 
        time_ = time;
    }
    ~EnergyDeposit() {};

    double GetEnergy() const {
        return energy_;
    };
    void SetEnergy(double value) {
        energy_ = value;
    };

    larnest::LArInteraction GetInteractionSpecies() const {
        return species_;
    }; 
    void SetInteractionSpecies(const larnest::LArInteraction value) {
        species_ = value;
    };

    Eigen::Vector3d GetPosition() const {
        return position_;
    };
    void SetPosition(const Eigen::Vector3d value) {
        position_ = value;
    };

    double GetStepLength() const {
        return dx_;
    };
    void SetStepLength(double value) {
        dx_ = value;
    };

    double GetTime() const {
        return time_;
    };
    void SetTime(double value) {
        time_ = value;
    };

    bool isEmpty() const {
        return energy_ <= 0 || dx_ <= 0 || time_ <= 0;
    };

private:
    double energy_;
    double dx_;
    larnest::LArInteraction species_;
    Eigen::Vector3d position_;
    double time_;
};

#endif // ENERGY_DEPOSIT_HH