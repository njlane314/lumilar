#ifndef IONISATION_HH
#define IONISATION_HH

#include <vector>
#include <iostream>
#include <Eigen/Dense>

#include "ThermalElectron.hh"
#include "Ejection.hh"
#include "Excitation.hh"

struct ElectronCloud {
    Eigen::Vector3d position;
    std::vector<ThermalElectron> electrons;
};

class Ionisation {
public:
    Ionisation();
    ~Ionisation();

    void AddCloud(const EnergyDeposit* energy_deposit, int cloud_size);
    std::vector<int> getCloudSizes() const;
    int getTotalElectronCount() const;
    std::vector<Eigen::Vector3d> getCloudPositions() const;
    void PrintIonisation() const;

private:
    static Ionisation* instance_;
    std::vector<ElectronCloud> ionisation_;
    Ejection ejection_generator_;
};

#endif // IONISATION_HH