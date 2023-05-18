#include "EventAction.hh"
//_________________________________________________________________________________________
EventAction::EventAction() {}
//_________________________________________________________________________________________
EventAction::~EventAction() {}
//_________________________________________________________________________________________
void EventAction::BeginOfEventAction(const G4Event* event) {}
//_________________________________________________________________________________________
void EventAction::EndOfEventAction(const G4Event* event) {
    auto signal = Signal::GetInstance();
    std::cout << "Beginning optical signal calculation..." << std::endl;
    AnalyticalOptics::CalculateOpticalSignal(signal, InstrumentConstruction::GetInstance()->GetOpticalSensors());
    std::cout << "Optical signal calculation complete." << std::endl;

    this->RunAnalysis(event, signal);
    signal->DeleteSignal(); 

    InstrumentConstruction::GetInstance()->ClearOpticalSensors();

    std::cout << "Updating progress bar..." << std::endl;
    this->UpdateProgressBar(event);
    std::cout << "Progress bar updated." << std::endl;
}
//_________________________________________________________________________________________
void EventAction::RunAnalysis(const G4Event* event, const Signal* signal) {
    //pulse_shape_->EventAnalysis(signal);
    //calorimetry_->EventAnalysis(signal);
    //coverage_->EventAnalysis(signal);
    //scintillation_collection_->EventAnalysis(signal);
    //ionisation_collection_->EventAnalysis(signal);
    geometric_acceptance_->EventAnalysis(signal);

    int events_to_generate = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    if (event->GetEventID() == events_to_generate - 1) {
        //pulse_shape_->RunAnalysis();
        //calorimetry_->RunAnalysis();
        //coverage_->RunAnalysis();
        //scintillation_collection_->RunAnalysis();
        //ionisation_collection_->RunAnalysis();
        geometric_acceptance_->RunAnalysis();
    }
}
//_________________________________________________________________________________________
void EventAction::UpdateProgressBar(const G4Event* event) {
    int event_idx = event->GetEventID();
    int events_to_generate = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

    if (events_to_generate < 100) {
        return;
    }
    
    int progress_interval = events_to_generate / 100.;

    if (event_idx % progress_interval == 0) {
        event_idx += 1;
        int percent_complete = event_idx * 100 / events_to_generate;
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
}