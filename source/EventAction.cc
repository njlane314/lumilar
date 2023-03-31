#include "EventAction.hh"

EventAction::EventAction()
: output_manager_(nullptr), verbose_level_(0), output_level_(0) {
	output_manager_ = OutputManager::Instance();
    analysis_manager_ = AnalysisManager::Instance();

    output_level_ = 2;
}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
    signal_ = new Signal();
}

void EventAction::EndOfEventAction(const G4Event* event) {
    if (verbose_level_ > 0) {
        PrintEvent(event);
    }

    int event_id = event->GetEventID() + 1;
	events_to_generate_ = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
	progress_interval_ = events_to_generate_ / 100.;
    if (event_id % progress_interval_ == 0) {
        int percent_complete = event_id * 100 / events_to_generate_;
        
        std::stringstream message;
        message << "[" << percent_complete << "%] " << "Generating event " << event_id << " of " << events_to_generate_;
        std::cout << message.str() << std::endl;
        std::cout.flush();
    }

    //output_manager_->RecordEntry(event); 
    
    analysis_manager_->DiscreteResponse(event, signal_->get_scintillation(), signal_->get_ionisation());
    analysis_manager_->EventResponse(event, signal_->get_scintillation(), signal_->get_ionisation());
    analysis_manager_->SignalYield(signal_->get_scintillation(), signal_->get_ionisation());

    analysis_manager_->StackPulseShape(signal_->get_scintillation());

    signal_->delete_signal();
}

void EventAction::PrintEvent(const G4Event* event) {}