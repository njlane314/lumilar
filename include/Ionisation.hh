#ifndef IONISATION_HH
#define IONISATION_HH

#include <vector>
#include <iostream>

#include "ThermalElectron.hh"
#include "Ejection.hh"

struct ElectronCloud {
    std::vector<double> position;
    std::vector<ThermalElectron> electrons;
};

class Ionisation {
 public:
    Ionisation();
    ~Ionisation();

    void add_cloud(int cloud_size, const std::vector<double>& position);
    std::vector<std::vector<double>> get_cloud_positions() const;
    std::vector<double> get_cloud_sizes() const;
    void print_ionisation() const;

 private:
    static Ionisation* instance_;
    std::vector<ElectronCloud> ionisation_;
    Ejection ejection_generator_;
};

#endif // IONISATION_HH