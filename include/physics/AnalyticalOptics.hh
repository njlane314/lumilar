#ifndef ANALYTICALOPTICS_HH
#define ANALYTICALOPTICS_HH

#include "Scintillation.hh"
#include "OpticalSensor.hh"
#include "MediumProperties.hh"
#include "Signal.hh"

#include "G4SystemOfUnits.hh"

#include <Eigen/Core>
#include <thread>
#include <vector>
#include <algorithm>
#include <cmath>
#include <omp.h>

class AnalyticalOptics {
public:
    AnalyticalOptics();
    ~AnalyticalOptics();

    static double GeometricQuenching(const PhotonRadiant* a_photon_radiant, const OpticalSensor* sensor);
    static double CalculateSolidAngle(const OpticalSensor* sensor, const Eigen::Vector3d* separation);
    static Eigen::Vector3d CreateProjectionGeometry(const OpticalSensor* sensor, const Eigen::Vector3d* separation);
    static double RectangularSolidAngle(const Eigen::Vector3d* projection, const double& width, const double& height);
    static void CalculateOpticalSignal(const Signal* signal, const OpticalSensorVector& optical_sensors);
    static OpticalPhoton CreateArrivalPhoton(const PhotonRadiant* photon_radiant, const OpticalPhoton& optical_photon, const OpticalSensor* optical_sensor);

private:
    static double AbsorptionQuenching(double distance);
};

#endif // ANALYTICALOPTICS_HH
