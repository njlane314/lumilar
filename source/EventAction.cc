#include "EventAction.hh"

EventAction::EventAction()
: output_manager_(nullptr), verbose_level_(0) {
	output_manager_ = OutputManager::Instance();
}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {}

void EventAction::EndOfEventAction(const G4Event* event) {
    if (verbose_level_ > 0) {
        PrintEvent(event);
    }

    int event_id = event->GetEventID() + 1;
	events_to_generate_ = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
	progress_interval_ = events_to_generate_ / 10.;
    if (event_id % progress_interval_ == 0) {
        int percent_complete = event_id * 100 / events_to_generate_;
        
        std::stringstream message;
        message << "[" << percent_complete << "%] " << "Generating event " << event_id << " of " << events_to_generate_;
        std::cout << message.str() << std::endl;
        std::cout.flush();
    }

    output_manager_->RecordEntry(event);
}

void EventAction::PrintEvent(const G4Event* event) {}