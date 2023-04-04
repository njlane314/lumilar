#include "EventAction.hh"

EventAction::EventAction() {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
    new Signal();
}

void EventAction::EndOfEventAction(const G4Event* event) {
    auto signal = Signal::get_instance();
    auto analysis_manager = AnalysisManager::Instance();
    auto output_manager = OutputManager::Instance();

    output_manager->RecordEntry(event);
    output_manager->RecordEntry(signal->get_scintillation(), signal->get_ionisation());
    
    analysis_manager->DiscreteResponse(signal->get_scintillation(), signal->get_ionisation());
    analysis_manager->EventResponse(event, signal, signal->get_scintillation(), signal->get_ionisation());
    analysis_manager->SignalYield(signal, signal->get_scintillation(), signal->get_ionisation());

    analysis_manager->StackPulseShape(signal->get_scintillation());
    analysis_manager->RandomPulseShape(signal->get_scintillation());
    analysis_manager->PulseShapeDiscrimination(signal->get_scintillation(), signal->get_ionisation());

    signal->delete_signal();

    // update progress bar
    int event_idx = event->GetEventID();
    events_to_generate_ = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    progress_interval_ = events_to_generate_ / 100.;

    if (event_idx % progress_interval_ == 0) {
        UpdateProgressBar(event_idx + 1, events_to_generate_, progress_interval_);
    }
}

void EventAction::UpdateProgressBar(int event_id, int events_to_generate, double progress_interval) {
    int percent_complete = event_id * 100 / events_to_generate;
    int bar_width = 50;
    int num_completed_chars = percent_complete * bar_width / 100;
    int num_remaining_chars = bar_width - num_completed_chars;

    std::stringstream message;
    message << "\r-- Generating events: [";
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
        std::cout << std::endl << "-- Event generation complete!" << std::endl;
    }
}