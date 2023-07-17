#ifndef IONISATION_HH
#define IONISATION_HH

#include <Eigen/Dense>
#include <vector>
#include <iostream>

#include "EnergyDeposit.hh"
#include "ElectronCloud.hh"
#include "Ejection.hh"

class Ionisation {
public:
    Ionisation();
    ~Ionisation();

    void CreateCloud(const EnergyDeposit* energy_deposit, int thermal_electron_count);
    std::vector<ElectronCloud> GetElectronClouds() const;
    std::vector<Eigen::Vector3d> GetCloudPositions() const;
    std::vector<int> GetCloudCounts() const;
    int GetTotalElectronCount() const;

private:
    static Ionisation* instance_;
    std::vector<ElectronCloud> electron_cloud_vector_;
};

#endif // IONISATION_HH