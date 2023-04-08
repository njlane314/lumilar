#ifndef SENSOR_CONSTRUCTION_H
#define SENSOR_CONSTRUCTION_H

#include <stdexcept>
#include <vector>
#include <Eigen/Core>
#include <variant>

#include "OpticalSensor.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"

class SensorConstruction {
public:
    SensorConstruction();
    ~SensorConstruction();
    static SensorConstruction* GetInstance();

    double detector_width_;
    double detector_height_;
    double detector_depth_;

    void ConstructRectangularOpticalSensors(PlaneOrientation plane_orientation, double sensor_separation, double sensor_width, double sensor_height);
    const OpticalSensorVector& GetOpticalSensors();

private:
    static SensorConstruction* instance_;
    OpticalSensorVector optical_sensors_;

    void ConstructPlane(const PlaneOrientation& orientation, double& plane_width, double& plane_height, Eigen::Vector3d& plane_center);
};

#endif  // SENSOR_CONSTRUCTION_H