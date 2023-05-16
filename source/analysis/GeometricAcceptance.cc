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
        TH2F_run_plots_.CreateHistogram(geometric_acceptance_distance_hist_name.str(), "Anode plane distance", "Geometric acceptance", 350, -1750, 1750, 50, 0, 0.5);
        geometric_acceptance_distance_hist = TH2F_run_plots_.GetHistogram(geometric_acceptance_distance_hist_name.str());
    }

    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
    int detected_photons = 0;
    for (const auto& optical_sensor : optical_sensors) {
        detected_photons += optical_sensor->GetPhotonCount();
    }

    int total_photons = signal->GetScintillation()->GetTotalPhotonCount();
    double geometric_frac = (double)detected_photons / (double)total_photons;
    
    std::vector<EnergyDeposit>* hit_vector = signal->GetHits();
    double x_energy = 0.;
    double total_energy = 0.;
    for (const auto& a_hit : *hit_vector) {
        double x_distance = a_hit.GetPosition().x();
        double hit_energy = a_hit.GetVisibleEnergy();
        x_energy += x_distance * hit_energy;
        total_energy += hit_energy;
    }
    double weighted_distance = x_energy / total_energy;

    geometric_acceptance_distance_hist->Fill(weighted_distance, geometric_frac);
}