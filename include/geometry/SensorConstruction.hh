#ifndef SENSOR_CONSTRUCTION_H
#define SENSOR_CONSTRUCTION_H

#include <stdexcept>
#include <vector>
#include <Eigen/Core>
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include <variant>

class SensorConstruction {
public:
    enum class PlaneOrientation {
        X_POSITIVE,
        X_NEGATIVE,
        Y_POSITIVE,
        Y_NEGATIVE,
        Z_POSITIVE,
        Z_NEGATIVE
    };
    
    class SensorShape {
    public:
        virtual double GetArea() const = 0;
    };

    class SquareSensor : public SensorShape {
    public:
        double size;
        double GetArea() const override { return size * size; }
    };

    class RectangularSensor : public SensorShape {
    public:
        double width;
        double height;
        double GetArea() const override { return width * height; }
    };

    class CircularSensor : public SensorShape {
    public:
        double radius;
        double GetArea() const override { return M_PI * radius * radius; }
    };
    
    struct OpticalSensor {
        std::variant<std::unique_ptr<SquareSensor>, std::unique_ptr<RectangularSensor>, std::unique_ptr<CircularSensor>> shape;
        Eigen::Vector3d position;
        PlaneOrientation orientation;
    };

    SensorConstruction();
    ~SensorConstruction();
    static SensorConstruction* GetInstance();

    double detector_width_;
    double detector_height_;
    double detector_depth_;

    void ConstructSquareOpticalSensors(PlaneOrientation plane_orientation, double sensor_separation, double sensor_width, double sensor_height);

    const std::vector<OpticalSensor *> GetOpticalSensors();

private:
    static SensorConstruction* instance_;
    std::vector<OpticalSensor*> optical_sensors_;

    void ConstructPlane(const PlaneOrientation& orientation, double& plane_width, double& plane_height, Eigen::Vector3d& plane_center);
};

#endif  // SENSOR_CONSTRUCTION_H
