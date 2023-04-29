#ifndef INSTRUMENT_CONSTRUCTION_HH
#define INSTRUMENT_CONSTRUCTION_HH

#include <stdexcept>
#include <vector>
#include <Eigen/Core>
#include <variant>

#include "OpticalSensor.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"

class InstrumentConstruction {
public:
    InstrumentConstruction();
    ~InstrumentConstruction();
    static InstrumentConstruction* GetInstance();

    double detector_width_;
    double detector_height_;
    double detector_depth_;

    void ConstructRectangularOpticalSensors(PlaneOrientation plane_orientation,  double sensor_width_separation, double sensor_height_separation, double sensor_width, double sensor_height);
    const OpticalSensorVector& GetOpticalSensors();
    int GetNumOpticalSensors() const;

private:
    static InstrumentConstruction* instance_;
    OpticalSensorVector optical_sensors_;

    void ConstructPlane(const PlaneOrientation& orientation, double& plane_width, double& plane_height, Eigen::Vector3d& plane_center);
};

#endif  // INSTRUMENT_CONSTRUCTION_HH