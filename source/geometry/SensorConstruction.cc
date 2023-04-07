#include "SensorConstruction.hh"

SensorConstruction* SensorConstruction::instance_ = nullptr;

SensorConstruction::SensorConstruction() {
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

void SensorConstruction::ConstructSquareOpticalSensors(PlaneOrientation plane_orientation, double sensor_separation, double sensor_width, double sensor_height) {
    if (sensor_width > sensor_separation || sensor_height > sensor_separation) {
        throw std::invalid_argument("-- Invalid sensor dimensions");
    }

    double plane_width, plane_height;
    Eigen::Vector3d plane_center;
    ConstructPlane(plane_orientation, plane_width, plane_height, plane_center);

    const int num_sensors_width = floor(plane_width / sensor_separation);
    const int num_sensors_height = floor(plane_height / sensor_separation);

    for (int i = 0; i < num_sensors_width; i++) {
        for (int j = 0; j < num_sensors_height; j++) {
            OpticalSensor* optical_sensor = new OpticalSensor();
            optical_sensor->position = plane_center;
            double along_width = (i * sensor_separation) + (sensor_separation / 2) - (plane_width / 2);
            double along_height = (j * sensor_separation) + (sensor_separation / 2) - (plane_height / 2);
            switch (plane_orientation) {
            case PlaneOrientation::X_POSITIVE:
            case PlaneOrientation::X_NEGATIVE:
                optical_sensor->position.z() = along_width;
                optical_sensor->position.y() = along_height;
                break;
            case PlaneOrientation::Y_POSITIVE:
            case PlaneOrientation::Y_NEGATIVE:
                optical_sensor->position.x() = along_width;
                optical_sensor->position.z() = along_height;
                break;
            case PlaneOrientation::Z_POSITIVE: 
            case PlaneOrientation::Z_NEGATIVE:
                optical_sensor->position.x() = along_width;
                optical_sensor->position.y() = along_height;
                break;
            default:
                throw std::invalid_argument("-- Invalid sensor orientation");
                break;
            }
            
            optical_sensors_.push_back(optical_sensor);
            delete optical_sensor;
        }
    }
}

void SensorConstruction::ConstructPlane(const PlaneOrientation& orientation, double& plane_width, double& plane_height, Eigen::Vector3d& plane_center) {
    const double half_width = detector_width_ / 2;
    const double half_height = detector_height_ / 2;
    const double half_depth = detector_depth_ / 2;

    switch (orientation) {
        case PlaneOrientation::X_POSITIVE:
            plane_width = detector_depth_;
            plane_height = detector_height_;
            plane_center = { half_width, 0, 0 };
            break;
        case PlaneOrientation::X_NEGATIVE:
            plane_width = detector_depth_;
            plane_height = detector_height_;
            plane_center = { -half_width, 0, 0 };
            break;
        case PlaneOrientation::Y_POSITIVE:
            plane_width = detector_width_;
            plane_height = detector_depth_;
            plane_center = { 0, half_height, 0 };
            break;
        case PlaneOrientation::Y_NEGATIVE:
            plane_width = detector_width_;
            plane_height = detector_depth_;
            plane_center = { 0, -half_height, 0 };
            break;
        case PlaneOrientation::Z_POSITIVE:
            plane_width = detector_width_;
            plane_height = detector_height_;
            plane_center = { 0, 0, half_depth };
            break;
        case PlaneOrientation::Z_NEGATIVE:
            plane_width = detector_width_;
            plane_height = detector_height_;
            plane_center = { 0, 0, -half_depth };
            break;
        default:
            throw std::invalid_argument("-- Invalid sensor orientation");
            break;
    }
}

const std::vector<SensorConstruction::OpticalSensor*> SensorConstruction::GetOpticalSensors() {
    return optical_sensors_;
}