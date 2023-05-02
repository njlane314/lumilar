#include "PulseShape.hh"

//SPARKS (Scintillation Profile Analysis while Recognising Kinetics)

AnalysisResults<TH1F> PulseShape::TH1F_evt_plots_;
AnalysisResults<TProfile> PulseShape::TProfile_evt_plots_;
AnalysisResults<TH2F> PulseShape::TH2F_run_plots_;

void PulseShape::EventAnalysis(const Signal* signal){
    int evt_id = CLHEP::RandFlat::shootInt(1000);
    PlotEmissionTimes(signal, evt_id);
    //PlotAmplitudeRatio(signal, evt_id);
    PlotArrivalTimes(evt_id);
} 

void PulseShape::RunAnalysis() {
    TH2F_run_plots_.SaveHistograms();
}

void PulseShape::PlotEmissionTimes(const Signal* signal, int evt_id) {
    std::vector<double> emission_times = signal->GetScintillation()->GetEmissionTimes();
    std::stringstream emission_hist_name;
    emission_hist_name << "evt" << std::setfill('0') << std::setw(3) << evt_id << "_emission_times";

    double x_min = 0.;
    double x_max = 3000.;
    double time_res = 1.;
    int n_bins = round((x_max - x_min)/time_res);

    TH1F_evt_plots_.CreateHistogram(emission_hist_name.str(), "Time of emission [ns]", "Optical photons/bin", n_bins, x_min, x_max);
    TH1F* evt_hist = TH1F_evt_plots_.GetHistogram(emission_hist_name.str());
    for (const auto& time : emission_times) {
        evt_hist->Fill(time);
    }

    // plot auto-correlation
    std::stringstream auto_corr_hist_name;
    auto_corr_hist_name << "evt" << std::setfill('0') << std::setw(3) << evt_id << "_auto_correlation";

    TProfile_evt_plots_.CreateHistogram(auto_corr_hist_name.str(), "Lag Time [ns]", "Auto-correlation", 2 * n_bins, -x_max, x_max);
    auto auto_corr_hist = TProfile_evt_plots_.GetHistogram(auto_corr_hist_name.str());
    for (int t_lag = -x_max; t_lag < x_max; t_lag += time_res) {
        double auto_corr = 0.;
        for (int bin = 1; bin <= n_bins; bin++) {
            if (bin * time_res + t_lag >= x_min && bin * time_res + t_lag < x_max) {
                auto_corr += evt_hist->GetBinContent(bin) * evt_hist->GetBinContent(bin + t_lag / time_res);
            }
        }
        auto_corr_hist->Fill(t_lag, auto_corr);
    }

    TProfile_evt_plots_.SaveHistograms();
    TH1F_evt_plots_.SaveHistograms();
}

void PulseShape::PlotArrivalTimes(int evt_id) {
    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
    std::stringstream arrival_hist_name;
    arrival_hist_name << "evt" << std::setfill('0') << std::setw(3) << evt_id << "_arrival_times";

    double x_min = 0.;
    double x_max = 3000.;
    double time_res = 1.;
    int n_bins = round((x_max - x_min)/time_res);

    TH1F_evt_plots_.CreateHistogram(arrival_hist_name.str(), "Time of arrival [ns]", "Optical photons/ns", n_bins, x_min, x_max);
    TH1F* evt_hist = TH1F_evt_plots_.GetHistogram(arrival_hist_name.str());
    for (const auto& sensor : optical_sensors) {
        for (const auto& time : sensor->GetPhotonTimes()) {
            evt_hist->Fill(time);
        }
    }

    TH1F_evt_plots_.SaveHistograms();
}

void PulseShape::PlotAmplitudeRatio(const Signal* signal, int evt_id) {
    std::vector<double> emission_times = signal->GetScintillation()->GetEmissionTimes();
    std::vector<double> decay_times = signal->GetDelayTimes();

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
        
        if (decay_times[0] > time_window) {
            if (time > decay_times[0] && time < decay_times[0] + time_window) {
                delayed_sum += 1.;
            }
        } else {
            return;
        }
    }

    double amplitude_ratio = delayed_sum / prompt_sum;

    std::string amplitude_ratio_hist_name = "amplitude_ratio";

    TH2F* amplitude_ratio_hist = TH2F_run_plots_.GetHistogram(amplitude_ratio_hist_name);

    if (amplitude_ratio_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(amplitude_ratio_hist_name, "Neutrino energy [MeV]", "Amplitude ratio", 100, 1, 0, 100, 1, 0);
        amplitude_ratio_hist = TH2F_run_plots_.GetHistogram(amplitude_ratio_hist_name);
    }

    amplitude_ratio_hist->Fill(signal->GetPrimaryEnergy(), amplitude_ratio);

    //////////

    std::string delayed_sum_hist_name = "delayed_sum";

    TH2F* delayed_sum_hist = TH2F_run_plots_.GetHistogram(delayed_sum_hist_name);

    if (delayed_sum_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(delayed_sum_hist_name, "Neutrino energy [MeV]", "L2", 100, 1, 0);
        delayed_sum_hist = TH2F_run_plots_.GetHistogram(delayed_sum_hist_name);
    }

    delayed_sum_hist->Fill(signal->GetPrimaryEnergy(), delayed_sum);

    /////////

    std::string prompt_sum_hist_name = "prompt_sum";

    TH2F* prompt_sum_hist = TH2F_run_plots_.GetHistogram(prompt_sum_hist_name);

    if (prompt_sum_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(prompt_sum_hist_name, "Neutrino energy [MeV]", "L1", 100, 1, 0);
        prompt_sum_hist = TH2F_run_plots_.GetHistogram(prompt_sum_hist_name);
    }

    prompt_sum_hist->Fill(signal->GetPrimaryEnergy(), prompt_sum);

    /////////

    /////////

    std::string prompt_sum_greater_hist_name = "prompt_sum_greater_16000";

    TH2F* prompt_sum_greater_hist = TH2F_run_plots_.GetHistogram(prompt_sum_greater_hist_name);

    if (prompt_sum_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(prompt_sum_greater_hist_name, "Neutrino energy [MeV]", "L1", 100, 1, 0);
        prompt_sum_greater_hist = TH2F_run_plots_.GetHistogram(prompt_sum_greater_hist_name);
    }

    if (delayed_sum > 16000.) {
        prompt_sum_greater_hist->Fill(signal->GetPrimaryEnergy(), prompt_sum);
    }

    /////////

    std::string prompt_sum_lesser_hist_name = "prompt_sum_lesser_16000";

    TH2F* prompt_sum_lesser_hist = TH2F_run_plots_.GetHistogram(prompt_sum_lesser_hist_name);

    if (prompt_sum_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(prompt_sum_lesser_hist_name, "Neutrino energy [MeV]", "L1", 100, 1, 0);
        prompt_sum_hist = TH2F_run_plots_.GetHistogram(prompt_sum_lesser_hist_name);
    }

    if (delayed_sum < 16000.) {
        prompt_sum_lesser_hist->Fill(signal->GetPrimaryEnergy(), prompt_sum);
    }
}