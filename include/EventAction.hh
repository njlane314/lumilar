#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include "InstrumentConstruction.hh"
#include "Optics.hh"
#include "Signal.hh"

#include "HitDataHandler.hh"

class EventAction : public G4UserEventAction {
public:
	EventAction();
  	virtual ~EventAction();

  	void BeginOfEventAction(const G4Event* event);
  	void EndOfEventAction(const G4Event* event);

private:
	void UpdateProgressBar(const G4Event* event);
	void RecordHit(const G4Event* event);

	Signal* signal_;
};

#endif // EVENT_ACTION_HH
