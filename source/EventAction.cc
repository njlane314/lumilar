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
        int bar_width = 50;
        int num_completed_chars = percent_complete * bar_width / 100;
        int num_remaining_chars = bar_width - num_completed_chars;

        std::stringstream message;
        message << "\rGenerating events: [";
        for (int i = 0; i < num_completed_chars; i++) {
            message << "=";
        }
        message << ">";
        for (int i = 0; i < num_remaining_chars - 1; i++) {
            message << " ";
        }
        message << "] " << std::setw(3) << percent_complete << "% complete.";
        std::cout << message.str() << std::flush;
        
        if (percent_complete == 100) {
            std::cout << std::endl << "Event generation complete!" << std::endl;
        }
    }

    //output_manager_->RecordEntry(event); 
    
    analysis_manager_->DiscreteResponse(signal_->get_scintillation(), signal_->get_ionisation());
    analysis_manager_->EventResponse(event, signal_, signal_->get_scintillation(), signal_->get_ionisation());
    analysis_manager_->SignalYield(signal_, signal_->get_scintillation(), signal_->get_ionisation());

    analysis_manager_->StackPulseShape(signal_->get_scintillation());
    analysis_manager_->RandomPulseShape(signal_->get_scintillation());
    analysis_manager_->PulseShapeDiscrimination(signal_->get_scintillation(), signal_->get_ionisation());

    signal_->delete_signal();
}

void EventAction::PrintEvent(const G4Event* event) {}