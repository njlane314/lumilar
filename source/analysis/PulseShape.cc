#include "PulseShape.hh"

//SPARKS (Scintillation Profile Analysis while Recognising Kinetics)

AnalysisResults<TH1F> PulseShape::TH1F_evt_plots_;
AnalysisResults<TH2F> PulseShape::TProfile_evt_plots_;
AnalysisResults<TH2F> PulseShape::TH2F_run_plots_;

void PulseShape::eventAnalysis(const Signal* signal){
    int evt_id = CLHEP::RandFlat::shootInt(1000);
    plotEmissionTimes(signal, evt_id);
    plotAmplitudeRatio(signal, evt_id);
} 

void PulseShape::runAnalysis() {
    TH2F_run_plots_.saveHistograms();
}

void PulseShape::plotEmissionTimes(const Signal* signal, int evt_id) {
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

void PulseShape::plotAmplitudeRatio(const Signal* signal, int evt_id) {
    std::vector<double> emission_times = signal->get_scintillation()->get_emission_times();
    std::vector<double> decay_times = signal->get_delay_times();

    double time_window = 30.; // ns
    double prompt_sum = 0.;
    double delayed_sum = 0.;

    if (decay_times.size() == 0) {
        return;
    }

    for (auto time : emission_times) {
        if (time > 0 && time < time_window) {
            prompt_sum += 1.;
        }
        
        if (time > decay_times[0] && time < decay_times[0] + time_window) {
            delayed_sum += 1.;
        }
    }

    double amplitude_ratio = delayed_sum / prompt_sum;
    std::cout << "Amplitude ratio: " << amplitude_ratio << " for event " << evt_id << " with decay time " << decay_times[0] << std::endl;

    std::string amplitude_ratio_hist_name = "amplitude_ratio";

    TH2F* amplitude_ratio_hist = TH2F_run_plots_.getHistogram(amplitude_ratio_hist_name);

    if (amplitude_ratio_hist == nullptr) {
        TH2F_run_plots_.createHistogram(amplitude_ratio_hist_name, "Neutrino energy [MeV]", "Amplitude ratio", 100, 1, 0, 100, 1, 0);
        amplitude_ratio_hist = TH2F_run_plots_.getHistogram(amplitude_ratio_hist_name);
    }

    amplitude_ratio_hist->Fill(signal->get_primary_energy(), amplitude_ratio);
}