#include "DetectorConstruction.hh"
//_________________________________________________________________________________________
#include "G4UIparameter.hh"
//_________________________________________________________________________________________`
DetectorConstruction::DetectorConstruction(DetectorMessenger* detector_messenger) : detector_messenger_(detector_messenger) {}
//_________________________________________________________________________________________
DetectorConstruction::~DetectorConstruction() {}
//_________________________________________________________________________________________
G4VPhysicalVolume* DetectorConstruction::Construct() { 
    detector_messenger_->SetDetectorParameters(detector_name_, detector_shape_, detector_width_, detector_height_, detector_depth_, detector_step_, detector_nitrogen_, detector_optical_placements_);
    std::string detector_material = "G4_lAr";

    G4Box* detector_box = new G4Box(detector_name_.c_str(), detector_width_/2., detector_height_/2., detector_depth_/2.);
    G4LogicalVolume* detector_logical = new G4LogicalVolume(detector_box, G4NistManager::Instance()->FindOrBuildMaterial(detector_material), "detector.logical");

    G4UserLimits* user_limits = new G4UserLimits();
    user_limits->SetMaxAllowedStep(detector_step_);
    detector_logical->SetUserLimits(user_limits);

    this->ReadOpticalSensorPlacements();

    G4ThreeVector detector_position(0.0, 0.0, 0.0);
    G4Transform3D detector_transform(G4Translate3D(detector_position) * G4Rotate3D());
    G4VPhysicalVolume* detector_physical = new G4PVPlacement(detector_transform, detector_logical, "detector.physical", 0, false, 0);
    
    G4VisAttributes* vis_attributes = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8)); 
    vis_attributes->SetVisibility(true);
    detector_logical->SetVisAttributes(vis_attributes); 

    return detector_physical;
}
//_________________________________________________________________________________________
void DetectorConstruction::GetDetectorDimensions(double& width, double& height, double& depth) {
    width = detector_width_;
    height = detector_height_;
    depth = detector_depth_;
}
//_________________________________________________________________________________________
void DetectorConstruction::ReadOpticalSensorPlacements() {
    std::cout << "-- Loading optical sensor placements..." << std::endl;
    sensor_types_.clear();
    sensor_positions_.clear();
    sensor_dimensions_.clear();

    std::cout << "-- Optical sensor placements file: " << detector_optical_placements_ << std::endl;

    std::ifstream optical_sensor_placements_file;
    optical_sensor_placements_file.open(detector_optical_placements_.c_str());
    if (!optical_sensor_placements_file.is_open()) {
        std::cout << "-- Unable to open optical sensor placements file." << std::endl;
        exit(1);
    }

    std::string dimensions_line;
    std::getline(optical_sensor_placements_file, dimensions_line);
    std::istringstream dimensions_iss(dimensions_line);
    double width, height, depth;
    if (dimensions_iss >> width >> height >> depth) {
        if (width != detector_width_ || height != detector_height_ || depth != detector_depth_) {
            std::cout << "-- Error: Inconsistent detector dimensions in the file." << std::endl;
            exit(1);
        }
    } else {
        std::cout << "-- Error: Failed to read detector dimensions from the file." << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(optical_sensor_placements_file, line)) {
        std::istringstream iss(line);
        std::string type, orientation;
        double x, y, z;
        double width, height;
        if (iss >> type >> orientation >> x >> y >> z >> width >> height) {
            std::vector<std::string> type_vec({ type, orientation });
            std::vector<double> position({ x, y, z });
            std::vector<double> dimensions({ width, height });

            sensor_types_.push_back(type_vec);
            sensor_positions_.push_back(position);
            sensor_dimensions_.push_back(dimensions);

            std::cout << "-- Optical sensor placement: " << type << " " << orientation << " " << x << " " << y << " " << z << " " << width << " " << height << std::endl;
        }
    }

    optical_sensor_placements_file.close();
}
//_________________________________________________________________________________________
void DetectorConstruction::GetOpticalSensorPlacements(std::vector<std::vector<std::string>>& sensor_types, std::vector<std::vector<double>>& sensor_positions, std::vector<std::vector<double>>& sensor_dimensions) {
    sensor_types = sensor_types_;
    sensor_positions = sensor_positions_;
    sensor_dimensions = sensor_dimensions_;
}