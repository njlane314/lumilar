#ifndef ANALYTICALOPTICS_HH
#define ANALYTICALOPTICS_HH

#include <memory>
#include <stdexcept>

#include <Eigen/Dense>
#include "Scintillation.hh"
#include "SensorConstruction.hh"

class AnalyticalOptics {
public:
    AnalyticalOptics();
    virtual ~AnalyticalOptics();

    double GeometricQuenching(const PhotonRadiant* a_photon_radiant, const SensorConstruction::OpticalSensor* sensor);
    double RectangularSolidAngle(const Eigen::Vector3d* separation, const SensorConstruction::OpticalSensor* sensor);

private:
    double CalculateSolidAngle(const SensorConstruction::OpticalSensor* sensor, const Eigen::Vector3d* separation);
    Eigen::Vector3d CreateProjectionGeometry(const SensorConstruction::OpticalSensor* sensor, const Eigen::Vector3d* separation);

    double AbsorptionQuenching(double distance);
}; 
#endif // ANALYTICALOPTICS_HH