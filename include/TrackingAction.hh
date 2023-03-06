#ifndef TRACKING_ACTION_HH
#define TRACKING_ACTION_HH

#include "G4UserTrackingAction.hh"

#include "OutputManager.hh"

#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

class TrackingAction : public G4UserTrackingAction {
public:
    TrackingAction();
    ~TrackingAction();

    void PreUserTrackingAction(const G4Track* track);
    void PostUserTrackingAction(const G4Track* track);
    
    void PrintTrack(const G4Track* track);

private:
    OutputManager* output_manager_;
    G4int verbose_level_;
};

#endif // TRACKING_ACTION_HH

