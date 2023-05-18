#include "GeometricAcceptance.hh"
//_________________________________________________________________________________________
AnalysisResults<TH2F> GeometricAcceptance::TH2F_run_plots_;
//_________________________________________________________________________________________
void GeometricAcceptance::EventAnalysis(const Signal* signal) {
    PlotGeometricAcceptanceDistance(signal);
}
//_________________________________________________________________________________________
void GeometricAcceptance::RunAnalysis() {
    TH2F_run_plots_.SaveHistograms();
}
//_________________________________________________________________________________________
void GeometricAcceptance::PlotGeometricAcceptanceDistance(const Signal* signal) {
    std::stringstream geometric_acceptance_distance_hist_name;
    geometric_acceptance_distance_hist_name << "geometric_acceptance_distance";

    TH2F* geometric_acceptance_distance_hist = TH2F_run_plots_.GetHistogram(geometric_acceptance_distance_hist_name.str());
    if (geometric_acceptance_distance_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(geometric_acceptance_distance_hist_name.str(), "Drift plane position [mm]", "Geometric acceptance", 35, -1750, 1750, 50, 0, 0.5);
        geometric_acceptance_distance_hist = TH2F_run_plots_.GetHistogram(geometric_acceptance_distance_hist_name.str());
    }

    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
    if (optical_sensors.empty()) {
        std::cout << "Error: No optical sensors available." << std::endl;
        return;
    }

    int detected_photons = 0;
    for (const auto& optical_sensor : optical_sensors) {
        detected_photons += optical_sensor->GetPhotonCount();
    }

    int total_photons = signal->GetScintillation()->GetTotalPhotonCount();
    if (total_photons == 0) {
        std::cout << "Error: No total photons available." << std::endl;
        return;
    }

    double geometric_frac = (double)detected_photons / (double)total_photons;
    
    std::vector<EnergyDeposit>* hit_vector = signal->GetHits();
    if (hit_vector == nullptr || hit_vector->empty()) {
        std::cout << "Error: No hits available." << std::endl;
        return;
    }

    double x_energy = 0.0;
    double total_energy = 0.0;
    for (const auto& a_hit : *hit_vector) {
        double x_distance = a_hit.GetPosition().x();
        double hit_energy = a_hit.GetVisibleEnergy();
        x_energy += x_distance * hit_energy;
        total_energy += hit_energy;
    }

    if (total_energy == 0.0) {
        std::cout << "Error: Total energy is zero." << std::endl;
        return;
    }

    double weighted_distance = x_energy / total_energy;

    if (geometric_acceptance_distance_hist != nullptr) {
        geometric_acceptance_distance_hist->Fill(weighted_distance, geometric_frac);
    } else {
        std::cout << "Error: Histogram not found." << std::endl;
    }
}
