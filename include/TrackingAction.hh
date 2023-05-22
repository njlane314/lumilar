#ifndef TRACKING_ACTION_HH
#define TRACKING_ACTION_HH

#include "G4UserTrackingAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include "TruthManager.hh"

class TrackingAction : public G4UserTrackingAction {
public:
    TrackingAction();
    ~TrackingAction();

    void PreUserTrackingAction(const G4Track* track);
    void PostUserTrackingAction(const G4Track* track);
};

#endif // TRACKING_ACTION_HH

