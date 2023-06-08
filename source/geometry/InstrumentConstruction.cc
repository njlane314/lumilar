#include "InstrumentConstruction.hh"
//_________________________________________________________________________________________
InstrumentConstruction* InstrumentConstruction::instance_ = nullptr;
//_________________________________________________________________________________________
InstrumentConstruction::InstrumentConstruction() {
    std::cout << "-- Constructing instruments" << std::endl;
    DetectorConstruction* detector_construction = (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
    detector_construction->GetDetectorDimensions(detector_width_, detector_height_, detector_depth_);
    detector_construction->GetOpticalSensorPlacements(sensor_types_, sensor_positions_, sensor_dimensions_);
}
//_________________________________________________________________________________________
InstrumentConstruction::~InstrumentConstruction() {}
//_________________________________________________________________________________________
InstrumentConstruction* InstrumentConstruction::GetInstance() {
    if (!instance_) {
        instance_ = new InstrumentConstruction();
    }
    
    return instance_;
}
//_________________________________________________________________________________________
void InstrumentConstruction::CreateOpticalSensors() {
    if (sensor_types_.size() == 0 || sensor_positions_.size() == 0 || sensor_dimensions_.size() == 0) {
        std::cout << "-- No input optical sensors to construct, constructing a unfirom placemnt on andoe plane" << std::endl;
        double rectangular_dimension = 100; // mm
        CreateUniformRectangularOpticalSensors(PlaneOrientation::X_POS, rectangular_dimension, rectangular_dimension, rectangular_dimension/2., rectangular_dimension/2.);
    } 

    if (sensor_types_.size() != sensor_positions_.size() || sensor_types_.size() != sensor_dimensions_.size()) {
        throw std::invalid_argument("-- Invalid sensor placements");
    } else {
        for (int i = 0; i < sensor_types_.size(); ++i) {
            std::vector<std::string> type_vec = sensor_types_[i];
            std::vector<double> placement = sensor_positions_[i];
            std::vector<double> dimensions = sensor_dimensions_[i];

            if (type_vec.size() != 2 || placement.size() != 3 || dimensions.size() != 2) {
                throw std::invalid_argument("-- Invalid sensor placements");
            }

            std::string type = type_vec[0];
            std::string orientation = type_vec[1];
            double width = dimensions[0];
            double height = dimensions[1];
            Eigen::Vector3d position(placement[0], placement[1], placement[2]);

            std::unique_ptr<OpticalSensor> optical_sensor;

            if (type == "rectangle") {
                if (orientation == "+x") {
                    optical_sensor = OpticalSensor::CreateRectangle(width, height, position, PlaneOrientation::X_POS);
                } else if (orientation == "-x") {
                    optical_sensor = OpticalSensor::CreateRectangle(width, height, position, PlaneOrientation::X_NEG);
                } else if (orientation == "+y") {
                    optical_sensor = OpticalSensor::CreateRectangle(width, height, position, PlaneOrientation::Y_POS);
                } else if (orientation == "-y") {
                    optical_sensor = OpticalSensor::CreateRectangle(width, height, position, PlaneOrientation::Y_NEG);
                } else if (orientation == "+z") {
                    optical_sensor = OpticalSensor::CreateRectangle(width, height, position, PlaneOrientation::Z_POS);
                } else if (orientation == "-z") {
                    optical_sensor = OpticalSensor::CreateRectangle(width, height, position, PlaneOrientation::Z_NEG);
                } else {
                    throw std::invalid_argument("-- Invalid sensor placement orientation");
                }
            } else {
                throw std::invalid_argument("-- Invalid sensor placement type");
            }

            optical_sensors_.push_back(std::move(optical_sensor));
        }
    }
}
//_________________________________________________________________________________________
void InstrumentConstruction::CreateUniformRectangularOpticalSensors(PlaneOrientation plane_orientation, double sensor_width_separation, double sensor_height_separation, double sensor_width, double sensor_height) {
    std::cout << "-- Constructing rectangular optical sensors" << std::endl;
    if (sensor_width > sensor_width_separation || sensor_height > sensor_height_separation) {
        throw std::invalid_argument("-- Invalid sensor dimensions");
    }

    double plane_width, plane_height;
    Eigen::Vector3d plane_center;
    ConstructPlane(plane_orientation, plane_width, plane_height, plane_center);

    const int num_sensors_width = floor(plane_width / sensor_width_separation);
    const int num_sensors_height = floor(plane_height / sensor_height_separation);

    for (int i = 0; i < num_sensors_width; ++i) {
        for (int j = 0; j < num_sensors_height; ++j) {
            Eigen::Vector3d sensor_position = plane_center;
            double along_width = (i * sensor_width_separation) + (sensor_width_separation / 2) - (plane_width / 2);
            double along_height = (j * sensor_height_separation) + (sensor_height_separation / 2) - (plane_height / 2);
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
            std::unique_ptr<OpticalSensor> optical_sensor = OpticalSensor::CreateRectangle(sensor_width, sensor_height, sensor_position, plane_orientation);
            optical_sensors_.push_back(std::move(optical_sensor));
        }
    }
}
//_________________________________________________________________________________________
void InstrumentConstruction::ConstructPlane(const PlaneOrientation& orientation, double& plane_width, double& plane_height, Eigen::Vector3d& plane_center) {
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
//_________________________________________________________________________________________
const OpticalSensorVector& InstrumentConstruction::GetOpticalSensors() {
    return optical_sensors_;
}
//_________________________________________________________________________________________
void InstrumentConstruction::ClearOpticalSensors() {
    for (const auto& sensor : optical_sensors_) {
        sensor->ClearPhotons();
    }
}
//_________________________________________________________________________________________
int InstrumentConstruction::GetTotalArrivalPhotons() {
    int photon_count = 0;
    for (const auto& sensor : optical_sensors_) {
        photon_count += sensor->GetPhotonCount();
    }
    return photon_count;
}
//_________________________________________________________________________________________
int InstrumentConstruction::GetNumOpticalSensors() const {
    int num_sensors = 0;
    for (const auto& sensor : optical_sensors_) {
        num_sensors += 1;
    }
    return num_sensors;
}