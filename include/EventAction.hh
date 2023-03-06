#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"

#include "G4Event.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include "OutputManager.hh"

class EventAction : public G4UserEventAction {
 public:
	EventAction();
  	virtual ~EventAction();

  	void BeginOfEventAction(const G4Event* event);
  	void EndOfEventAction(const G4Event* event);

 private:
	void PrintEvent(const G4Event* event);

	OutputManager* output_manager_;

	int verbose_level_;
	std::string source_;

	int events_to_generate_;
	int progress_interval_;
};

#endif // EVENT_ACTION_HH
