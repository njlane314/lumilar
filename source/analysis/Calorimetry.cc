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
        TH2F_run_plots_.createHistogram(scint_yield_hist_name.str(), "Electronic Linear Energy Transfer [MeV / cm]", "Scintillation Yield [/ MeV]", 100, 1, 0, 100, 1, 0);
        TH2F_run_plots_.createHistogram(ionis_yield_hist_name.str(), "Electronic Linear Energy Transfer [MeV / cm]", "Ionisation Yield [/ MeV]", 100, 1, 0, 100, 1, 0);
        scint_yield_hist = TH2F_run_plots_.getHistogram(scint_yield_hist_name.str());
        ionis_yield_hist = TH2F_run_plots_.getHistogram(ionis_yield_hist_name.str());
    }

    auto energy_deposit_vec = signal->get_visible_deposits();
    auto radiant_size_vec = signal->get_scintillation()->get_radiant_sizes();
    auto cloud_size_vec = signal->get_ionisation()->get_cloud_sizes();
    auto linear_transfer_vec = signal->get_linear_transfers();
    for (int i = 0; i < energy_deposit_vec.size(); ++i) {
        auto energy_deposit = energy_deposit_vec[i];
        auto radiant_size = radiant_size_vec[i];
        auto cloud_size = cloud_size_vec[i];
        auto linear_transfer = linear_transfer_vec[i];
        
        if (linear_transfer > 0) {
            scint_yield_hist->Fill(linear_transfer, (double)radiant_size / (double)energy_deposit);
            ionis_yield_hist->Fill(linear_transfer, (double)cloud_size / (double)energy_deposit);
        }
    }
}

void Calorimetry::runAnalysis() {
    TH2F_run_plots_.saveHistograms();
}