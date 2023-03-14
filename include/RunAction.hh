#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include <chrono>

#include "G4UserRunAction.hh"

#include "OutputManager.hh"
#include "MaterialProperties.hh"

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();
    
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);

    void PrintRun();
    
private:
    OutputManager* output_manager_;

    int verbose_level_;

    std::chrono::system_clock::time_point start_time_;
};

#endif // RUN_ACTION_HH

