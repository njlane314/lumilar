#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "Signal.hh"
#include "OutputManager.hh"

#include "G4UserEventAction.hh"

#include "G4Event.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include "AnalysisManager.hh"
#include "PulseShapeOutput.hh"

#include "SensorConstruction.hh"
#include "AnalyticalOptics.hh"

class EventAction : public G4UserEventAction {
 public:
	EventAction();
  	virtual ~EventAction();

  	void BeginOfEventAction(const G4Event* event);
  	void EndOfEventAction(const G4Event* event);

 private:
	void PrintEvent(const G4Event* event);
	void UpdateProgressBar(int event_id, int events_to_generate, double progress_interval);
	OutputManager* output_manager_;
	AnalysisManager* analysis_manager_;
	PulseShapeOutput* pulse_shape_manager_;
	Signal* signal_;

	int verbose_level_;
	int output_level_;
	std::string source_;

	int events_to_generate_;
	int progress_interval_;
};

#endif // EVENT_ACTION_HH
