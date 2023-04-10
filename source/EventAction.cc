#include "EventAction.hh"

EventAction::EventAction() {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
    new Signal();
}

void EventAction::EndOfEventAction(const G4Event* event) {
    auto signal = Signal::get_instance();

    std::vector<PhotonRadiant> photon_radiants = signal->get_scintillation()->get_photon_radiants();
    const OpticalSensorVector& optical_sensors = SensorConstruction::GetInstance()->GetOpticalSensors();
    
    int num_photons;
    int num_photons_detected;
    for (const auto& a_optical_sensor : optical_sensors) {
        for (auto& a_photon_radiant : photon_radiants) {
            num_photons = a_photon_radiant.photons.size();
            num_photons_detected = AnalyticalOptics::GeometricQuenching(&a_photon_radiant, a_optical_sensor.get()) * a_photon_radiant.photons.size();
            //std::cout << "Geometric quenching factor: " << AnalyticalOptics::GeometricQuenching(&a_photon_radiant, a_optical_sensor.get()) << "\n";
        }
    }

    std::cout << "Number of photons detected: " << num_photons_detected << "\n";
    std::cout << "Number of photons: " << num_photons << "\n";
    std::cout << "Geometric quenching factor: " << num_photons_detected / num_photons << "\n";

    calorimetry_->processSignal(signal);

    signal->delete_signal();

    // update progress bar
    int event_idx = event->GetEventID();
    events_to_generate_ = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    progress_interval_ = events_to_generate_ / 100.;

    if (event_idx % progress_interval_ == 0) {
        UpdateProgressBar(event_idx + 1, events_to_generate_, progress_interval_);
    }

    if (event_idx == events_to_generate_ - 1) {
        calorimetry_->writeToFile("results.root");
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