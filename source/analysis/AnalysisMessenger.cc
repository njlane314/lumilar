#include "AnalysisMessenger.hh"
//_________________________________________________________________________________________
AnalysisMessenger::AnalysisMessenger() : G4UImessenger() {
    analysis_directory_ = new G4UIdirectory("/analysis/");
    analysis_directory_->SetGuidance("Analysis control commands.");

    analysis_hits_cmd_ = new G4UIcommand("/analysis/hits", this);
    analysis_hits_cmd_->SetGuidance("Set hits analysis.");
    analysis_hits_cmd_->SetGuidance("[usage] /analysis/hits <true/false>");

    analysis_calorimetry_cmd_ = new G4UIcommand("/analysis/calorimetry", this);
    analysis_calorimetry_cmd_->SetGuidance("Set calorimetry analysis.");
    analysis_calorimetry_cmd_->SetGuidance("[usage] /analysis/calorimetry <true/false>");

    analysis_pulse_shape_cmd_ = new G4UIcommand("/analysis/pulseshape", this);
    analysis_pulse_shape_cmd_->SetGuidance("Set pulse shape analysis.");
    analysis_pulse_shape_cmd_->SetGuidance("[usage] /analysis/pulseshape <true/false>");
}
//_________________________________________________________________________________________
void AnalysisMessenger::SetNewValue(G4UIcommand* cmd, G4String new_value) {
    if (cmd == analysis_hits_cmd_) {
        if (new_value.c_str() == "true") {
            hits_enabled_ = true;
        }
        else {
            hits_enabled_ = false;
        }
    }
    else if (cmd == analysis_calorimetry_cmd_) {
        if (new_value.c_str() == "true") {
            calorimetry_enabled_ = true;
        }
        else {
            calorimetry_enabled_ = false;
        }
    }
    else if (cmd == analysis_pulse_shape_cmd_) {
        if (new_value.c_str() == "true") {
            pulse_shape_enabled_ = true;
        }
        else {
            pulse_shape_enabled_ = false;
        }
    }
    else {
        std::cout << "-- AnalysisMessenger::SetNewValue: Unknown command!" << std::endl;
    }
}
//_________________________________________________________________________________________
bool AnalysisMessenger::GetHitsEnabled() const {
    return hits_enabled_;
}
//_________________________________________________________________________________________
bool AnalysisMessenger::GetCalorimetryEnabled() const {
    return calorimetry_enabled_;
}
//_________________________________________________________________________________________
bool AnalysisMessenger::GetPulseShapeEnabled() const {
    return pulse_shape_enabled_;
}