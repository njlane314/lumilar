#include "Calorimetry.hh"

AnalysisResults<TH2F> Calorimetry::TH2F_run_plots_;

void Calorimetry::eventAnalysis(const Signal* signal) {
    plotEnergyYields(signal);
}

void Calorimetry::plotEnergyYields(const Signal* signal) {
    std::stringstream scint_yield_hist_name;
    std::stringstream ionis_yield_hist_name;
    scint_yield_hist_name << "scintillation_yield";
    ionis_yield_hist_name << "ionisation_yield";

    TH2F* scint_yield_hist = TH2F_run_plots_.getHistogram(scint_yield_hist_name.str());
    TH2F* ionis_yield_hist = TH2F_run_plots_.getHistogram(ionis_yield_hist_name.str());
    if (scint_yield_hist == nullptr) {
        TH2F_run_plots_.createHistogram(scint_yield_hist_name.str(), "Energy Deposit [MeV]", "Scintillation Yield [/ MeV]", 100, 1, 0, 100, 1, 0);
        TH2F_run_plots_.createHistogram(ionis_yield_hist_name.str(), "Energy Deposit [MeV]", "Ionisation Yield [/ MeV]", 100, 1, 0, 100, 1, 0);
        scint_yield_hist = TH2F_run_plots_.getHistogram(scint_yield_hist_name.str());
        ionis_yield_hist = TH2F_run_plots_.getHistogram(ionis_yield_hist_name.str());
    }

    auto visible_deposits_vec = signal->getVisibleDeposits();
    auto linear_transfer_vec = signal->getLinearTransfers();

    double total_energy_dep = 0.;
    auto total_photons = signal->getScintillation()->getTotalPhotonCount();
    auto total_electrons = signal->getIonisation()->getTotalElectronCount();

    for (auto visible_deposit : visible_deposits_vec) {
        total_energy_dep += visible_deposit;
    }
    
    scint_yield_hist->Fill(total_energy_dep, (double)total_photons / (double)total_energy_dep);
    ionis_yield_hist->Fill(total_energy_dep, (double)total_electrons / (double)total_energy_dep);
}

void Calorimetry::runAnalysis() {
    TH2F_run_plots_.saveHistograms();
}