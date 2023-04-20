#include "EventAction.hh"

EventAction::EventAction(bool signal_physics)
: signal_physics_(signal_physics) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {}

void EventAction::EndOfEventAction(const G4Event* event) {
    if (signal_physics_ == true) {
        auto signal = Signal::get_instance();

        this->runAnalysis(event, signal);
        signal->delete_signal();
        delete signal;
        signal = nullptr;
    }

    //std::vector<PhotonRadiant> photon_radiants = signal->get_scintillation()->get_photon_radiants();
    //const OpticalSensorVector& optical_sensors = SensorConstruction::GetInstance()->GetOpticalSensors();
    //AnalyticalOptics::CalculateOpticalSignal(signal, optical_sensors);

    //this->UpdateProgressBar(event);
}

void EventAction::runAnalysis(const G4Event* event, const Signal* signal) {
    pulse_shape_->eventAnalysis(signal);
    calorimetry_->eventAnalysis(signal);

    int events_to_generate = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    if (event->GetEventID() == events_to_generate - 1) {
        pulse_shape_->runAnalysis();
        calorimetry_->runAnalysis();
    }
}

void EventAction::UpdateProgressBar(const G4Event* event) {
    int event_idx = event->GetEventID();
    int events_to_generate = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    
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