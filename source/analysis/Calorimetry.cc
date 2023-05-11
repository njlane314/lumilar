#include "Calorimetry.hh"
//_________________________________________________________________________________________
AnalysisResults<TH2F> Calorimetry::TH2F_run_plots_;
//_________________________________________________________________________________________
void Calorimetry::EventAnalysis(const Signal* signal) {
    PlotEnergyYields(signal);
}
//_________________________________________________________________________________________
void Calorimetry::PlotEnergyYields(const Signal* signal) {
    std::stringstream scint_yield_hist_name;
    std::stringstream ionis_yield_hist_name;
    scint_yield_hist_name << "scintillation_yield";
    ionis_yield_hist_name << "ionisation_yield";

    TH2F* scint_yield_hist = TH2F_run_plots_.GetHistogram(scint_yield_hist_name.str());
    TH2F* ionis_yield_hist = TH2F_run_plots_.GetHistogram(ionis_yield_hist_name.str());
    if (scint_yield_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(scint_yield_hist_name.str(), "Energy Deposit [MeV]", "Scintillation Yield [/ MeV]", 100, 1, 0, 100, 1, 0);
        TH2F_run_plots_.CreateHistogram(ionis_yield_hist_name.str(), "Energy Deposit [MeV]", "Ionisation Yield [/ MeV]", 100, 1, 0, 100, 1, 0);
        scint_yield_hist = TH2F_run_plots_.GetHistogram(scint_yield_hist_name.str());
        ionis_yield_hist = TH2F_run_plots_.GetHistogram(ionis_yield_hist_name.str());
    }

    auto visible_deposits_vec = signal->GetVisibleDeposits();
    auto linear_transfer_vec = signal->GetLinearTransfers();

    double total_energy_dep = 0.;
    auto total_photons = signal->GetScintillation()->GetTotalPhotonCount();
    auto total_electrons = signal->GetIonisation()->GetTotalElectronCount();

    for (auto visible_deposit : visible_deposits_vec) {
        total_energy_dep += visible_deposit;
    }
    
    scint_yield_hist->Fill(total_energy_dep, (double)total_photons / (double)total_energy_dep);
    ionis_yield_hist->Fill(total_energy_dep, (double)total_electrons / (double)total_energy_dep);
}
//_________________________________________________________________________________________
void Calorimetry::RunAnalysis() {
    TH2F_run_plots_.SaveHistograms();
}