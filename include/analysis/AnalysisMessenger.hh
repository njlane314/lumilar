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

    bool GetHitsOutputEnabled() const;

private:
    G4UIdirectory* output_directory_;
    G4UIcommand* output_hits_cmd_;

    bool output_hits_enabled_;
};

#endif  // ANALYSISMESSENGER_HH
