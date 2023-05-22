#include "Calorimetry.hh"
//_________________________________________________________________________________________
AnalysisResults<TH2F> Calorimetry::TH2F_run_plots_;
//_________________________________________________________________________________________
void Calorimetry::EventAnalysis(const Signal* signal) {
    PlotSignalYields(signal);
}
//_________________________________________________________________________________________
void Calorimetry::PlotSignalYields(const Signal* signal) {
    std::stringstream yields_hist_name;
    yields_hist_name << "signal_yields";

    TH2F* yields_hist = TH2F_run_plots_.GetHistogram(yields_hist_name.str());
    if (yields_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(yields_hist_name.str(), r'$N_e / E_{dep}$ [/ MeV]', r'N_\gamma / E_{dep} [/ MeV]', 100, 1, 0, 100, 1, 0);
        yields_hist = TH2F_run_plots_.GetHistogram(yields_hist_name.str());
    }

    auto visible_deposits_vec = signal->GetVisibleDeposits();
    auto total_photons = signal->GetScintillation()->GetTotalPhotonCount();
    auto total_electrons = signal->GetIonisation()->GetTotalElectronCount();

    double total_energy_dep = 0.;
    for (auto visible_deposit : visible_deposits_vec) {
        total_energy_dep += visible_deposit;
    }

    yields_hist->Fill((double)total_electrons / (double)total_energy_dep, (double)total_photons / (double)total_energy_dep);
}
//_________________________________________________________________________________________
void Calorimetry::RunAnalysis() {
    TH2F_run_plots_.SaveHistograms();
}