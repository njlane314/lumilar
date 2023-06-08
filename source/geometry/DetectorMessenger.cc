#include "DetectorMessenger.hh"
//_________________________________________________________________________________________
DetectorMessenger::DetectorMessenger() : G4UImessenger() {
    detector_directory_ = new G4UIdirectory("/detector/");
    detector_directory_->SetGuidance("Detector construction control commands.");

    detector_name_cmd_ = new G4UIcommand("/detector/name", this);
    detector_name_cmd_->SetGuidance("Set detector name.");
    detector_name_cmd_->SetGuidance("[usage] /detector/name <detector_name>");

    detector_shape_cmd_ = new G4UIcommand("/detector/shape", this);
    detector_shape_cmd_->SetGuidance("Set detector shape.");
    detector_shape_cmd_->SetGuidance("[usage] /detector/shape <detector_shape>");

    detector_width_cmd_ = new G4UIcommand("/detector/width", this);
    detector_width_cmd_->SetGuidance("Set detector width.");
    detector_width_cmd_->SetGuidance("[usage] /detector/width <detector_width [mm]>");

    detector_height_cmd_ = new G4UIcommand("/detector/height", this);
    detector_height_cmd_->SetGuidance("Set detector height.");
    detector_height_cmd_->SetGuidance("[usage] /detector/height <detector_heigh [mm]t>");

    detector_depth_cmd_ = new G4UIcommand("/detector/depth", this);
    detector_depth_cmd_->SetGuidance("Set detector depth.");
    detector_depth_cmd_->SetGuidance("[usage] /detector/depth <detector_depth [mm]>");

    detector_instruments_cmd_ = new G4UIcommand("/detector/instruments", this);
    detector_instruments_cmd_->SetGuidance("Set detector optical placements.");
    detector_instruments_cmd_->SetGuidance("[usage] /detector/instruments <detector_optical_placements>");

    detector_step_cmd_ = new G4UIcommand("/detector/step", this);
    detector_step_cmd_->SetGuidance("Set detector step.");
    detector_step_cmd_->SetGuidance("[usage] /detector/step <detector_step [mm]>");

    detector_xenon_cmd_ = new G4UIcommand("/detector/xenon", this);
    detector_xenon_cmd_->SetGuidance("Set detector xenon concentration.");
    detector_xenon_cmd_->SetGuidance("[usage] /detector/xenon <detector_xenon [ppm]>");

    detector_nitrogen_cmd_ = new G4UIcommand("/detector/nitrogen", this);
    detector_nitrogen_cmd_->SetGuidance("Set detector nitrogen concentration.");
    detector_nitrogen_cmd_->SetGuidance("[usage] /detector/nitrogen <detector_nitrogen [ppm]>");

    G4UIparameter* detector_name_parameter = new G4UIparameter("detector_name", 's', true);
    detector_name_parameter->SetGuidance("Set detector name.");
    detector_name_parameter->SetGuidance("name of the detector");
    detector_name_parameter->SetDefaultValue("detector");
    detector_name_cmd_->SetParameter(detector_name_parameter);

    G4UIparameter* detector_shape_parameter = new G4UIparameter("detector_shape", 's', true);
    detector_shape_parameter->SetGuidance("Set detector shape.");
    detector_shape_parameter->SetGuidance("shape of the detector");
    detector_shape_parameter->SetDefaultValue("box");
    detector_shape_cmd_->SetParameter(detector_shape_parameter);

    G4UIparameter* detector_width_parameter = new G4UIparameter("detector_width", 'd', true);
    detector_width_parameter->SetGuidance("Set detector width.");
    detector_width_parameter->SetDefaultValue(1.0);
    detector_width_cmd_->SetParameter(detector_width_parameter);

    G4UIparameter* detector_height_parameter = new G4UIparameter("detector_height", 'd', true);
    detector_height_parameter->SetGuidance("Set detector height.");
    detector_height_parameter->SetDefaultValue(1.0);
    detector_height_cmd_->SetParameter(detector_height_parameter);

    G4UIparameter* detector_depth_parameter = new G4UIparameter("detector_depth", 'd', true);
    detector_depth_parameter->SetGuidance("Set detector depth.");
    detector_depth_parameter->SetDefaultValue(1.0);
    detector_depth_cmd_->SetParameter(detector_depth_parameter);

    G4UIparameter* detector_step_parameters = new G4UIparameter("detector_step", 'd', true);
    detector_step_parameters->SetGuidance("Set detector step.");
    detector_step_parameters->SetDefaultValue(1.0);
    detector_step_cmd_->SetParameter(detector_step_parameters);

    G4UIparameter* detector_xenon_parameter = new G4UIparameter("detector_xenon", 'd', true);
    detector_xenon_parameter->SetGuidance("Set detector xenon concentration.");
    detector_xenon_parameter->SetDefaultValue(0.0);
    detector_xenon_cmd_->SetParameter(detector_xenon_parameter);

    G4UIparameter* detector_nitrogen_parameter = new G4UIparameter("detector_nitrogen", 'd', true);
    detector_nitrogen_parameter->SetGuidance("Set detector nitrogen concentration.");
    detector_nitrogen_parameter->SetDefaultValue(0.0);
    detector_nitrogen_cmd_->SetParameter(detector_nitrogen_parameter);

    G4UIparameter* detetcor_optical_placements_parameter = new G4UIparameter("detector_optical_placements", 's', true);
    detetcor_optical_placements_parameter->SetGuidance("Set detector optical placements.");
    detetcor_optical_placements_parameter->SetGuidance("optical placements of the detector");
    detetcor_optical_placements_parameter->SetDefaultValue("none");
    detector_instruments_cmd_->SetParameter(detetcor_optical_placements_parameter);
}
//_________________________________________________________________________________________
DetectorMessenger::~DetectorMessenger() {}
//_________________________________________________________________________________________
void DetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String new_value) {
    if (cmd == detector_name_cmd_) {
        detector_name_ = new_value.c_str();
    } else if (cmd == detector_shape_cmd_) {
        detector_shape_ = new_value.c_str();
    } else if (cmd == detector_width_cmd_) {
        detector_width_ = std::stod(new_value);  
    } else if (cmd == detector_height_cmd_) {
        detector_height_ = std::stod(new_value);
    } else if (cmd == detector_depth_cmd_) {
        detector_depth_ = std::stod(new_value);
    } else if (cmd == detector_step_cmd_) {
        detector_step_ = std::stod(new_value);
    } else if (cmd == detector_xenon_cmd_) {
        detector_xenon_ = std::stod(new_value);
    } else if (cmd == detector_nitrogen_cmd_) {
        detector_nitrogen_ = std::stod(new_value);
    } else if (cmd == detector_instruments_cmd_) {
        detector_instruments_ = new_value.c_str();
    } else {
        std::cout << "DetectorConstruction::SetNewValue: Unknown command" << std::endl;
    }
}
//_________________________________________________________________________________________
void DetectorMessenger::SetDetectorParameters(std::string& detector_name, std::string& detector_shape, double& detector_width, double& detector_height, double& detector_depth, double& detector_step, double& detector_xenon, double& detector_nitrogen, std::string& detector_instruments) {
    detector_name = detector_name_;
    detector_shape = detector_shape_;
    detector_width = detector_width_;
    detector_height = detector_height_;
    detector_depth = detector_depth_;
    detector_step = detector_step_;
    detector_xenon = detector_xenon_;
    detector_nitrogen = detector_nitrogen_;
    detector_instruments = detector_instruments_;
}