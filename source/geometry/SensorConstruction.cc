#include "SensorConstruction.hh"

SensorConstruction* SensorConstruction::instance_ = nullptr;

SensorConstruction::SensorConstruction() {
    std::cout << "-- Constructing sensor construction" << std::endl;
    DetectorConstruction* detector_construction = (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
    detector_construction->GetDetectorDimensions(detector_width_, detector_height_, detector_depth_);
}

SensorConstruction::~SensorConstruction() {}

SensorConstruction* SensorConstruction::GetInstance() {
    if (!instance_) {
        instance_ = new SensorConstruction();
    }
    
    return instance_;
}

void SensorConstruction::ConstructRectangularOpticalSensors(PlaneOrientation plane_orientation, double sensor_separation, double sensor_width, double sensor_height) {
    std::cout << "-- Constructing rectangular optical sensors" << std::endl;
    if (sensor_width > sensor_separation || sensor_height > sensor_separation) {
        throw std::invalid_argument("-- Invalid sensor dimensions");
    }

    double plane_width, plane_height;
    Eigen::Vector3d plane_center;
    ConstructPlane(plane_orientation, plane_width, plane_height, plane_center);

    const int num_sensors_width = floor(plane_width / sensor_separation);
    const int num_sensors_height = floor(plane_height / sensor_separation);

    for (int i = 0; i < num_sensors_width; ++i) {
        for (int j = 0; j < num_sensors_height; ++j) {
            Eigen::Vector3d sensor_position = plane_center;
            double along_width = (i * sensor_separation) + (sensor_separation / 2) - (plane_width / 2);
            double along_height = (j * sensor_separation) + (sensor_separation / 2) - (plane_height / 2);
            switch (plane_orientation) {
                case PlaneOrientation::X_POS:
                case PlaneOrientation::X_NEG:
                    sensor_position.z() = along_width;
                    sensor_position.y() = along_height;
                    break;
                case PlaneOrientation::Y_POS:
                case PlaneOrientation::Y_NEG:
                    sensor_position.x() = along_width;
                    sensor_position.z() = along_height;
                    break;
                case PlaneOrientation::Z_POS:
                case PlaneOrientation::Z_NEG:
                    sensor_position.x() = along_width;
                    sensor_position.y() = along_height;
                    break;
                default:
                    throw std::invalid_argument("-- Invalid sensor orientation");
            }
            std::unique_ptr<OpticalSensor> optical_sensor = OpticalSensor::createRectangle(sensor_width, sensor_height, sensor_position, plane_orientation);
            optical_sensors_.push_back(std::move(optical_sensor));
        }
    }
}

void SensorConstruction::ConstructPlane(const PlaneOrientation& orientation, double& plane_width, double& plane_height, Eigen::Vector3d& plane_center) {
    const double half_width = detector_width_ / 2;
    const double half_height = detector_height_ / 2;
    const double half_depth = detector_depth_ / 2;

    switch (orientation) {
        case PlaneOrientation::X_POS:
            plane_width = detector_depth_;
            plane_height = detector_height_;
            plane_center = { half_width, 0, 0 };
            break;
        case PlaneOrientation::X_NEG:
            plane_width = detector_depth_;
            plane_height = detector_height_;
            plane_center = { -half_width, 0, 0 };
            break;
        case PlaneOrientation::Y_POS:
            plane_width = detector_width_;
            plane_height = detector_depth_;
            plane_center = { 0, half_height, 0 };
            break;
        case PlaneOrientation::Y_NEG:
            plane_width = detector_width_;
            plane_height = detector_depth_;
            plane_center = { 0, -half_height, 0 };
            break;
        case PlaneOrientation::Z_POS:
            plane_width = detector_width_;
            plane_height = detector_height_;
            plane_center = { 0, 0, half_depth };
            break;
        case PlaneOrientation::Z_NEG:
            plane_width = detector_width_;
            plane_height = detector_height_;
            plane_center = { 0, 0, -half_depth };
            break;
        default:
            throw std::invalid_argument("-- Invalid sensor orientation");
            break;
    }
}

const OpticalSensorVector& SensorConstruction::GetOpticalSensors() {
    return optical_sensors_;
}

int SensorConstruction::GetNumOpticalSensors() const {
    return optical_sensors_.size();
}