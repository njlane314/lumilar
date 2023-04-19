#include "PulseShape.hh"

//SPARKS (Scintillation Profile Analysis while Recognising Kinetics)

AnalysisResults<TH1F> PulseShape::TH1F_evt_plots_;
AnalysisResults<TH2F> PulseShape::TProfile_evt_plots_;

void PulseShape::eventAnalysis(const Signal* signal){
    plotEmissionTimes(signal);
} 

void PulseShape::plotEmissionTimes(const Signal* signal) {
    int evt_id = CLHEP::RandFlat::shootInt(1000);

    std::vector<double> emission_times = signal->get_scintillation()->get_emission_times();
    std::stringstream emission_hist_name;
    emission_hist_name << "evt" << std::setfill('0') << std::setw(3) << evt_id << "_emission_times";

    double x_min = 0.;
    double x_max = 3000.;
    double time_res = 1.;
    int n_bins = round((x_max - x_min)/time_res);

    TH1F_evt_plots_.createHistogram(emission_hist_name.str(), "Time of emission [ns]", "Optical photons/bin", n_bins, x_min, x_max);
    TH1F* evt_hist = TH1F_evt_plots_.getHistogram(emission_hist_name.str());
    for (const auto& time : emission_times) {
        evt_hist->Fill(time);
    }

    // plot auto-correlation
    std::stringstream auto_corr_hist_name;
    auto_corr_hist_name << "evt" << std::setfill('0') << std::setw(3) << evt_id << "_auto_correlation";

    TProfile_evt_plots_.createHistogram(auto_corr_hist_name.str(), "Lag Time [ns]", "Auto-correlation", 2 * n_bins, -x_max, x_max);
    auto auto_corr_hist = TProfile_evt_plots_.getHistogram(auto_corr_hist_name.str());
    for (int t_lag = -x_max; t_lag < x_max; t_lag += time_res) {
        double auto_corr = 0.;
        for (int bin = 1; bin <= n_bins; bin++) {
            if (bin * time_res + t_lag >= x_min && bin * time_res + t_lag < x_max) {
                auto_corr += evt_hist->GetBinContent(bin) * evt_hist->GetBinContent(bin + t_lag / time_res);
            }
        }
        auto_corr_hist->Fill(t_lag, auto_corr);
    }

    TProfile_evt_plots_.saveHistograms();
    TH1F_evt_plots_.saveHistograms();
}
