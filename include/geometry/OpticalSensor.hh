#ifndef OPTICAL_SENSOR_HH
#define OPTICAL_SENSOR_HH

#include <cmath> 
#include <Eigen/Core> 
#include <memory>
#include <vector>

#include "OpticalPhoton.hh"

enum class PlaneOrientation {
    X_POS,
    X_NEG,
    Y_POS,
    Y_NEG,
    Z_POS,
    Z_NEG
};

class OpticalSensor {
public:
    class Shape {
    public:
        virtual ~Shape() {}
        virtual double area() const = 0;
    };

    class Rectangle : public Shape {
    public:
        Rectangle(double width, double height) : width_(width), height_(height) {}
        double area() const override { return width_ * height_; }
        double getWidth() const { return width_; }
        double getHeight() const { return height_; }
    private:
        double width_, height_;
    };

    class Circle : public Shape {
    public:
        Circle(double radius) : radius_(radius) {}
        double area() const override { return M_PI * radius_ * radius_; }
        double getRadius() const { return radius_; }
    private:
        double radius_;
    };

    OpticalSensor(std::unique_ptr<Shape> shape, const Eigen::Vector3d& position, const PlaneOrientation& orientation) :
        shape_(std::move(shape)), position_(position), orientation_(orientation) {}

    double getArea() const {
        return shape_->area();
    }

    const Shape* getShape() const {
        return shape_.get();
    }

    const Eigen::Vector3d& getPosition() const {
        return position_;
    }

    const PlaneOrientation& getOrientation() const {
        return orientation_;
    }

    static std::unique_ptr<OpticalSensor> createRectangle(double width, double height, const Eigen::Vector3d& position, const PlaneOrientation& orientation) {
        return std::make_unique<OpticalSensor>(std::make_unique<Rectangle>(width, height), position, orientation);
    }

    static std::unique_ptr<OpticalSensor> createCircle(double radius, const Eigen::Vector3d& position, const PlaneOrientation& orientation) {
        return std::make_unique<OpticalSensor>(std::make_unique<Circle>(radius), position, orientation);
    }

    std::pair<double, double> getDimensions() const {
        if (auto rectangleShape = dynamic_cast<const OpticalSensor::Rectangle*>(shape_.get())) {
            return std::make_pair(rectangleShape->getWidth(), rectangleShape->getHeight());
        } else {
            return std::make_pair(0.0, 0.0);
        }
    }

    bool isRectangular() const {
        const OpticalSensor::Shape* shape = getShape();
        const OpticalSensor::Rectangle* rectangleShape = dynamic_cast<const OpticalSensor::Rectangle*>(shape);
        return (rectangleShape != nullptr);
    }

    void addPhoton(const OpticalPhoton& photon) {
        detected_photons_.push_back(photon);
    }

    const std::vector<OpticalPhoton>& getPhotons() const {
        return detected_photons_;
    }

    const std::vector<double> getPhotonTimes() const {
        std::vector<double> photon_times;
        for (const auto& photon : detected_photons_) {
            photon_times.push_back(photon.get_emission_time());
        }
        return photon_times;
    }

private:
    std::unique_ptr<Shape> shape_;
    Eigen::Vector3d position_;
    PlaneOrientation orientation_;
    std::vector<OpticalPhoton> detected_photons_;
};

using OpticalSensorVector = std::vector<std::unique_ptr<OpticalSensor>>;

#endif  // OPTICAL_SENSOR_HH
