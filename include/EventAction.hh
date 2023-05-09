#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "Signal.hh"

#include "G4UserEventAction.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include "InstrumentConstruction.hh"
#include "AnalyticalOptics.hh"
#include "Calorimetry.hh"
#include "PulseShape.hh"
#include "Coverage.hh"

class EventAction : public G4UserEventAction {
public:
	EventAction(bool is_signal_physics);
  	virtual ~EventAction();

  	void BeginOfEventAction(const G4Event* event);
  	void EndOfEventAction(const G4Event* event);

private:
	void PrintEvent(const G4Event* event);
	void UpdateProgressBar(const G4Event* event);
	void RunAnalysis(const G4Event* event, const Signal* signal);

	Signal* signal_;
	Calorimetry* calorimetry_;
	PulseShape* pulse_shape_;
	Coverage* coverage_;

	bool is_signal_physics_;
};

#endif // EVENT_ACTION_HH
