#ifndef OPTICAL_SENSOR_HH
#define OPTICAL_SENSOR_HH

#include <cmath> 
#include <Eigen/Core> 
#include <memory>
#include <vector>
#include <mutex>

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
        virtual double Area() const = 0;
    };

    class Rectangle : public Shape {
    public:
        Rectangle(double width, double height) : width_(width), height_(height) {}
        double Area() const override { return width_ * height_; }
        double GetWidth() const { return width_; }
        double GetHeight() const { return height_; }
    private:
        double width_, height_;
    };

    class Circle : public Shape {
    public:
        Circle(double radius) : radius_(radius) {}
        double Area() const override { return M_PI * radius_ * radius_; }
        double GetRadius() const { return radius_; }
    private:
        double radius_;
    };

    OpticalSensor(std::unique_ptr<Shape> shape, const Eigen::Vector3d& position, const PlaneOrientation& orientation) :
        shape_(std::move(shape)), position_(position), orientation_(orientation) {}

    double GetArea() const {
        return shape_->Area();
    }

    const Shape* GetShape() const {
        return shape_.get();
    }

    const Eigen::Vector3d& GetPosition() const {
        return position_;
    }

    const PlaneOrientation& GetOrientation() const {
        return orientation_;
    }

    static std::unique_ptr<OpticalSensor> CreateRectangle(double width, double height, const Eigen::Vector3d& position, const PlaneOrientation& orientation) {
        return std::make_unique<OpticalSensor>(std::make_unique<Rectangle>(width, height), position, orientation);
    }

    static std::unique_ptr<OpticalSensor> CreateCircle(double radius, const Eigen::Vector3d& position, const PlaneOrientation& orientation) {
        return std::make_unique<OpticalSensor>(std::make_unique<Circle>(radius), position, orientation);
    }

    std::pair<double, double> GetDimensions() const {
        if (auto rectangleShape = dynamic_cast<const OpticalSensor::Rectangle*>(shape_.get())) {
            return std::make_pair(rectangleShape->GetWidth(), rectangleShape->GetHeight());
        } else {
            return std::make_pair(0.0, 0.0);
        }
    }

    bool IsRectangular() const {
        const OpticalSensor::Shape* shape = GetShape();
        const OpticalSensor::Rectangle* rectangleShape = dynamic_cast<const OpticalSensor::Rectangle*>(shape);
        return (rectangleShape != nullptr);
    }

    void AddPhoton(const OpticalPhoton& photon) {
        std::lock_guard<std::mutex> lock(mutex_);
        detected_photons_.push_back(photon);
    }

    const std::vector<OpticalPhoton>& GetPhotons() const {
        return detected_photons_;
    }

    const int GetPhotonCount() const {
        int count = 0;
        for (const auto& photon : detected_photons_) {
            count += 1;
        }
        return count;
    }

    const std::vector<double> GetPhotonTimes() const {
        std::vector<double> photon_times;
        for (const auto& photon : detected_photons_) {
            photon_times.push_back(photon.GetArrivalTime());
        }
        return photon_times;
    }

    void ClearPhotons() {
        detected_photons_.clear();
    }

private:
    std::unique_ptr<Shape> shape_;
    Eigen::Vector3d position_;
    PlaneOrientation orientation_;
    std::vector<OpticalPhoton> detected_photons_;
    std::mutex mutex_;
};

using OpticalSensorVector = std::vector<std::unique_ptr<OpticalSensor>>;

#endif  // OPTICAL_SENSOR_HH
