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

    void add_cloud(const EnergyDeposit* energy_deposit, int cloud_size);
    std::vector<Eigen::Vector3d> get_cloud_positions() const;
    std::vector<int> get_cloud_sizes() const;
    void print_ionisation() const;

    int get_total_electrons() const;

 private:
    static Ionisation* instance_;
    std::vector<ElectronCloud> ionisation_;
    Ejection ejection_generator_;
};

#endif // IONISATION_HH