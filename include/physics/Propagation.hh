#ifndef PROPAGATION_HH
#define PROPAGATION_HH

#include <vector>
#include <cmath>
#include <stdexcept>

#include "OpticalPhoton.hh"
#include "OpticalSensor.hh"
#include "MediumProperties.hh"
#include "Scintillation.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"

class Propagation {
public:
    Propagation();
    ~Propagation();

    static double SampleArrivalTime(const PhotonRadiant* photon_radiant, const OpticalPhoton& optical_photon, const OpticalSensor* optical_sensor, const double detector_width, const double detetcor_height, const double detector_depth);

private:
    static double FluxProbabilityDensity(double x, double x_0, double x_b, double y, double y_0, double y_b, double z, double z_0, double z_b, double t, PlaneOrientation plane_orient, double wl);
    static double CalculateDiffusion(const double wl);
    static double SingleDimensionDiffusion(double x, double x_0, double x_b, double t, double D, int accuracy_scale);
    static double SingleDimensionDiffusionDerivative(double x, double x_0, double x_b, double t, double D, int accuracy_scale);
};

#endif  // PROPAGATION_HH