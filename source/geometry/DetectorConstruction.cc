#include "DetectorConstruction.hh"
//_________________________________________________________________________________________
#include "G4UIparameter.hh"
//_________________________________________________________________________________________`
DetectorConstruction::DetectorConstruction(DetectorMessenger* detector_messenger) : detector_messenger_(detector_messenger) {}
//_________________________________________________________________________________________
DetectorConstruction::~DetectorConstruction() {}
//_________________________________________________________________________________________
G4VPhysicalVolume* DetectorConstruction::Construct() { 
    detector_messenger_->SetDetectorParameters(detector_name_, detector_shape_, detector_width_, detector_height_, detector_depth_, detector_step_, detector_xenon_, detector_nitrogen_);
    std::string detector_material = "G4_lAr";
    MediumProperties* medium_properties = new MediumProperties(detector_xenon_, detector_nitrogen_);

    G4Box* detector_box = new G4Box(detector_name_.c_str(), detector_width_/2., detector_height_/2., detector_depth_/2.);
    G4LogicalVolume* detector_logical = new G4LogicalVolume(detector_box, G4NistManager::Instance()->FindOrBuildMaterial(detector_material), "detector.logical");

    G4UserLimits* user_limits = new G4UserLimits();
    user_limits->SetMaxAllowedStep(detector_step_);
    detector_logical->SetUserLimits(user_limits);

    G4ThreeVector detector_position(0.0, 0.0, 0.0);
    G4Transform3D detector_transform(G4Translate3D(detector_position) * G4Rotate3D());
    G4VPhysicalVolume* detector_physical = new G4PVPlacement(detector_transform, detector_logical, "detector.physical", 0, false, 0);
    
    return detector_physical;
}
//_________________________________________________________________________________________
void DetectorConstruction::GetDetectorDimensions(double& width, double& height, double& depth) {
    width = detector_width_;
    height = detector_height_;
    depth = detector_depth_;
}