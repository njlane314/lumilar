#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction(std::string detector_config)
: detector_config_(detector_config) {}

DetectorConstruction::~DetectorConstruction() {}

DetectorConstruction::Detector DetectorConstruction::ReadDetector(std::stringstream& ss) {
    Detector detector;
    std::string key;
    std::getline(ss, key, ':');

    if (key == "Detector") {
        while (ss.good()) {
            std::string sub_key;
            ss >> sub_key;

            if (sub_key == "Name:") {
                ss >> detector.name;
            } else if (sub_key == "Material:") {
                ss >> detector.material;
            } else if (sub_key == "Shape:") {
                ss >> detector.shape;
            } else if (sub_key == "Width:") {
                ss >> detector.width;
            } else if (sub_key == "Height:") {
                ss >> detector.height;
            } else if (sub_key == "Depth:") {
                ss >> detector.depth;
            } else if (sub_key == "Position:") {
                ss >> detector.position[0] >> detector.position[1] >> detector.position[2];
            } else if (sub_key == "Step:") {
                ss >> detector.step;
            }
        }
    }
    
    return detector;
}


G4VPhysicalVolume* DetectorConstruction::Construct() { 
    std::ifstream config_file(detector_config_);
    std::stringstream ss;
    ss << config_file.rdbuf();
    detector_ = ReadDetector(ss);

    G4Box* detector_shape = new G4Box(detector_config_.c_str(), detector_.width/2., detector_.height/2., detector_.depth/2.);
    G4LogicalVolume* detector_logical = new G4LogicalVolume(detector_shape, G4NistManager::Instance()->FindOrBuildMaterial(detector_.material), "detector.logical");

    G4UserLimits* user_limits = new G4UserLimits();
    user_limits->SetMaxAllowedStep(detector_.step);

    detector_logical->SetUserLimits(user_limits);

    G4ThreeVector detector_position(detector_.position[0], detector_.position[1], detector_.position[2]);
    G4Transform3D detector_transform(G4Translate3D(detector_position) * G4Rotate3D());

    G4VPhysicalVolume* detector_physical = new G4PVPlacement(detector_transform, detector_logical, "detector.physical", 0, false, 0);

    return detector_physical;
}

void DetectorConstruction::GetDetectorDimensions(double& width, double& height, double& depth) {
    width = detector_.width;
    height = detector_.height;
    depth = detector_.depth;
}