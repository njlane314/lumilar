#include "ScintillationCollection.hh"
//_________________________________________________________________________________________
AnalysisResults<TH2F> ScintillationCollection::TH2F_run_plots_;
//_________________________________________________________________________________________
void ScintillationCollection::EventAnalysis(const Signal* signal) {
    PlotEmissionPhotons(signal);
    PlotAmplitudeRatio(signal);
    PlotCollectionFraction(signal);
}
//_________________________________________________________________________________________
void ScintillationCollection::RunAnalysis() {
    TH2F_run_plots_.SaveHistograms();
}
//_________________________________________________________________________________________
void ScintillationCollection::PlotEmissionPhotons(const Signal* signal) {
    int emitted_photons = signal->GetScintillation()->GetTotalPhotonCount();
    double primary_energy = signal->GetPrimaryEnergy();

    std::string collected_fraction_name = "emission_energy_hist";

    TH2F* emission_energy_hist = TH2F_run_plots_.GetHistogram(collected_fraction_name);

    if (emission_energy_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(collected_fraction_name, "Neutrino energy [MeV]", "Emission photons", 16, 0, 16, 45, 0, 450000);
        emission_energy_hist = TH2F_run_plots_.GetHistogram(collected_fraction_name);
    }

    emission_energy_hist->Fill(primary_energy, emitted_photons);
}
//_________________________________________________________________________________________
void ScintillationCollection::PlotAmplitudeRatio(const Signal* signal) {
    std::vector<double> emission_times = signal->GetScintillation()->GetEmissionTimes();
    std::vector<double> decay_times = signal->GetDelayTimes();

    double primary_energy = signal->GetPrimaryEnergy();

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

    amplitude_ratio_hist->Fill(primary_energy, amplitude_ratio);
}
//_________________________________________________________________________________________
void ScintillationCollection::PlotCollectionFraction(const Signal* signal) {
    int emitted_photons = signal->GetScintillation()->GetTotalPhotonCount();

    std::string collected_fraction_name = "collected_fraction_hist";

    TH2F* collected_fraction_hist = TH2F_run_plots_.GetHistogram(collected_fraction_name);

    if (collected_fraction_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(collected_fraction_name, "Collection fraction", "Collcted photons", 100, 0, 1, 450, 0, 450000);
        collected_fraction_hist = TH2F_run_plots_.GetHistogram(collected_fraction_name);
    }

    for (int effic = 0; effic < 1000; effic++) {
        double collection_efficiency = effic / 1000.;
        //int emission_fraction = CLHEP::RandPoisson::shoot(floor((double)emitted_photons * collection_efficiency));
        int emission_fraction = CLHEP::RandBinomial::shoot(emitted_photons, collection_efficiency);

        collected_fraction_hist->Fill(collection_efficiency, emission_fraction);
    } 
}