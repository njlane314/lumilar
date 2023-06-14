#ifndef ANALYSISMESSENGER_HH
#define ANALYSISMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"

class AnalysisMessenger : public G4UImessenger {
public:
    AnalysisMessenger();
    void SetNewValue(G4UIcommand* command, G4String newValue);

    bool GetHitsEnabled() const;
    bool GetCalorimetryEnabled() const;
    bool GetPulseShapeEnabled() const;

private:
    G4UIdirectory* analysis_directory_;
    G4UIcommand* analysis_calorimetry_cmd_;
    G4UIcommand* analysis_hits_cmd_;
    G4UIcommand* analysis_pulse_shape_cmd_;

    bool hits_enabled_;
    bool calorimetry_enabled_;
    bool pulse_shape_enabled_;
};

#endif  // ANALYSISMESSENGER_HH
