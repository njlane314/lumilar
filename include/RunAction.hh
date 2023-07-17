#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include <chrono>

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4LogicalVolumeStore.hh"

#include "InstrumentConstruction.hh"
#include "HitDataHandler.hh"
#include "TruthManager.hh"

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();
    
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);
    virtual void HandleHits(const G4Run* run);
    virtual void FillMetaData();
    
private:
    std::chrono::system_clock::time_point start_time_;
};

#endif // RUN_ACTION_HH