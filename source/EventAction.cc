#include "EventAction.hh"

EventAction::EventAction() {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
    signal_ = new Signal();
}

void EventAction::UpdateProgressBar(int event_id, int events_to_generate, double progress_interval) {
    int percent_complete = event_id * 100 / events_to_generate;
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

void EventAction::EndOfEventAction(const G4Event* event) {

    //output_manager_->RecordEntry(event); 
    //output_manager_->RecordEntry(signal_->get_scintillation(), signal_->get_ionisation());

    //pulse_shape_manager_->RecordEntry(signal_->get_scintillation());
    
    analysis_manager_->DiscreteResponse(signal_->get_scintillation(), signal_->get_ionisation());
    analysis_manager_->EventResponse(event, signal_, signal_->get_scintillation(), signal_->get_ionisation());
    analysis_manager_->SignalYield(signal_, signal_->get_scintillation(), signal_->get_ionisation());

    analysis_manager_->StackPulseShape(signal_->get_scintillation());
    analysis_manager_->RandomPulseShape(signal_->get_scintillation());
    analysis_manager_->PulseShapeDiscrimination(signal_->get_scintillation(), signal_->get_ionisation());

    signal_->delete_signal();

    // update progress bar
    int event_idx = event->GetEventID();
    events_to_generate_ = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    progress_interval_ = events_to_generate_ / 100.;

    if (event_idx % progress_interval_ == 0) {
        UpdateProgressBar(event_idx++, events_to_generate_, progress_interval_);
    }
}