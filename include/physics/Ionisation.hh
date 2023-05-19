//____________________________________________________________________________
/*!

\class   physics::Ionisation

\brief   This class manages the ionisation signal.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef IONISATION_HH
#define IONISATION_HH

#include <Eigen/Dense>
#include <vector>
#include <iostream>

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
    std::vector<int> GetCloudSizes() const;
    int GetTotalElectronCount() const;
    std::vector<Eigen::Vector3d> GetCloudPositions() const;

private:
    static Ionisation* instance_;
    std::vector<ElectronCloud> ionisation_;
    Ejection ejection_generator_;
};

#endif // IONISATION_HH