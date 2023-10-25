#ifndef OPTICS_HH
#define OPTICS_HH

#include <Eigen/Core>
#include <thread>
#include <vector>
#include <algorithm>
#include <cmath>
#include <omp.h>
#include <chrono>

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include "Relaxation.hh"
#include "OpticalSensor.hh"
#include "Scintillation.hh"
#include "Signal.hh"
#include "ThreadPool.hh"
#include "PropagationTime.hh"
#include "PhotonRadiant.hh"

class Optics {
public:
    Optics();
    ~Optics();

    static void CalculateOpticalSignal(const Signal* signal, const OpticalSensorVector* optical_sensors);
    
    static void ProcessRadiant(const PhotonRadiant& radiant, const OpticalSensorVector* optical_sensors);

    static double GeometricQuenching(const OpticalSensor* sensor, const Eigen::Vector3d* separation);
    static double AbsorptionQuenching(const double distance);
    static double SampleArrivalTime(const double initial_time, const double distance, const double angle, const std::string particle_name);
    
    static double CalculateSolidAngle(const OpticalSensor* sensor, const Eigen::Vector3d* separation);
    static Eigen::Vector3d CreateProjectionGeometry(const OpticalSensor* sensor, const Eigen::Vector3d* separation);
    static double RectangularSolidAngle(const Eigen::Vector3d* projection, const double& width, const double& height);
    
    static void PrintDebug(std::chrono::time_point<std::chrono::high_resolution_clock> start_time);

private:
    static ThreadPool optics_thread_pool_;
    
    static bool debug_status_;
    static int debug_emission_photons_;
    static double debug_geometric_acceptance_;
    static int debug_photons_collected_;
};

#endif // OPTICS_HH