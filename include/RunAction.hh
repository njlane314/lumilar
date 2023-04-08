#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include <chrono>

#include "G4UserRunAction.hh"

#include "OutputManager.hh"
#include "AnalysisManager.hh"
#include "PulseShapeOutput.hh"
#include "SensorConstruction.hh"

#include "MaterialProperties.hh"

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();
    
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);
    void PrintRun();
    
private:
    std::chrono::system_clock::time_point start_time_;
};

#endif // RUN_ACTION_HH

