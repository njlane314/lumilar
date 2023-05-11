//____________________________________________________________________________
/*!

\class   geometry::InstrumentConstruction

\brief   This class constructs the detector instrumentation.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef INSTRUMENT_CONSTRUCTION_HH
#define INSTRUMENT_CONSTRUCTION_HH

#include <stdexcept>
#include <vector>
#include <Eigen/Core>
#include <variant>

#include "G4RunManager.hh"

#include "DetectorConstruction.hh"
#include "OpticalSensor.hh"

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
    int GetTotalArrivalPhotons();
    void ClearOpticalSensors();

private:
    static InstrumentConstruction* instance_;
    OpticalSensorVector optical_sensors_;

    void ConstructPlane(const PlaneOrientation& orientation, double& plane_width, double& plane_height, Eigen::Vector3d& plane_center);
};

#endif  // INSTRUMENT_CONSTRUCTION_HH