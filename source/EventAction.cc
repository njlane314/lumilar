#include "EventAction.hh"
//_________________________________________________________________________________________
EventAction::EventAction() {}
//_________________________________________________________________________________________
EventAction::~EventAction() {}
//_________________________________________________________________________________________
void EventAction::BeginOfEventAction(const G4Event* event) {}
//_________________________________________________________________________________________
void EventAction::EndOfEventAction(const G4Event* event) {
    AnalysisManager* analysis_manager = AnalysisManager::GetInstance();

    this->RecordHit(event);

    Signal* signal = Signal::GetInstance(); 
    Optics::CalculateOpticalSignal(signal, &InstrumentConstruction::GetInstance()->GetOpticalSensors());

    HitDataHandler* hit_data_handler = HitDataHandler::GetInstance();

    hit_data_handler->AddSignal(signal);
    
    this->UpdateProgressBar(event);

    InstrumentConstruction::GetInstance()->ClearOpticalSensors();
    signal->DeleteSignal();
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
//_________________________________________________________________________________________
void EventAction::RecordHit(const G4Event* event) {
    int mod = event->GetEventID() % 1000;
    if (mod == 0) {
        G4cout << "Event " << event->GetEventID() << "..." << G4endl;
    }

    HitDataHandler* hit_data_handler = HitDataHandler::GetInstance();
    hit_data_handler->SetEvent(event->GetEventID());


    hit_data_handler->EventFill();
    hit_data_handler->EventReset();
}