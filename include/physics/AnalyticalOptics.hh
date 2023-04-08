#ifndef ANALYTICALOPTICS_HH
#define ANALYTICALOPTICS_HH

#include <memory>
#include <stdexcept>

#include <Eigen/Dense>
#include "SensorConstruction.hh"


class AnalyticalOptics {
public:
    AnalyticalOptics();
    virtual ~AnalyticalOptics();

    double GeometricQuenching(const PhotonRadiant* a_photon_radiant, const OpticalSensor* sensor);
    double RectangularSolidAngle(const Eigen::Vector3d& separation, const OpticalSensor& sensor);

private:
    double CalculateSolidAngle(const OpticalSensor& sensor, const Eigen::Vector3d& separation);
    Eigen::Vector3d CreateProjectionGeometry(const OpticalSensor& sensor, const Eigen::Vector3d& separation);

    double AbsorptionQuenching(double distance);
} 
#endif // ANALYTICALOPTICS_HH