#include "AnalysisMessenger.hh"
//_________________________________________________________________________________________
AnalysisMessenger::AnalysisMessenger() : G4UImessenger() {
    output_directory_ = new G4UIdirectory("/output/");
    output_directory_->SetGuidance("Output control commands.");

    output_hits_cmd_ = new G4UIcommand("/output/hits", this);
    output_hits_cmd_->SetGuidance("Enable hits output.");
    output_hits_cmd_->SetGuidance("[usage] /output/hits <true/false>");
}
//_________________________________________________________________________________________
void AnalysisMessenger::SetNewValue(G4UIcommand* cmd, G4String new_value) {
    if (cmd == output_hits_cmd_) {
        if (new_value.c_str() == "true") {
            output_hits_enabled_ = true;
        }
        else {
            output_hits_enabled_ = false;
        }
    }
    else {
        std::cout << "-- AnalysisMessenger::SetNewValue: Unknown command!" << std::endl;
    }
}
//_________________________________________________________________________________________
bool AnalysisMessenger::GetHitsOutputEnabled() const {
    return output_hits_enabled_;
}