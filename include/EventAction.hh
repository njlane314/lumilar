#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include "InstrumentConstruction.hh"
#include "AnalyticalOptics.hh"
#include "Signal.hh"

#include "Calorimetry.hh"
#include "PulseShape.hh"
#include "Coverage.hh"
#include "ScintillationCollection.hh"
#include "IonisationCollection.hh"
#include "GeometricAcceptance.hh"

class EventAction : public G4UserEventAction {
public:
	EventAction();
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
	ScintillationCollection* scintillation_collection_;
	IonisationCollection* ionisation_collection_;
	GeometricAcceptance* geometric_acceptance_;
};

#endif // EVENT_ACTION_HH
