//____________________________________________________________________________
/*!

\class   geometry::AnalyticalOptics

\brief   This class contains analytical methods for calculating the transportation effects
        of optical photons in the detector.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef ANALYTICALOPTICS_HH
#define ANALYTICALOPTICS_HH

#include <Eigen/Core>
#include <thread>
#include <vector>
#include <algorithm>
#include <cmath>
#include <omp.h>
#include <chrono>

#include "G4SystemOfUnits.hh"

#include "OpticalSensor.hh"
#include "Scintillation.hh"
#include "Signal.hh"
#include "MediumProperties.hh"
#include "ThreadPool.hh"

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
