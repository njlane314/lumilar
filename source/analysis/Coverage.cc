#include "Coverage.hh"
//_________________________________________________________________________________________
AnalysisResults<TH1F> Coverage::TH1F_evt_plots_;
AnalysisResults<TH2F> Coverage::TH2F_run_plots_;
AnalysisResults<TH1F> Coverage::TH1F_run_plots_;
//_________________________________________________________________________________________
void Coverage::EventAnalysis(const Signal* signal) {
    int evt_id = CLHEP::RandFlat::shootInt(1000);
    //PlotDetectorPhotons(evt_id);
    //PlotGeometricFraction(signal);
    PlotHistFraction(signal);
    PlotAnodeFraction(signal);
}
//_________________________________________________________________________________________
void Coverage::RunAnalysis() {
    TH1F_run_plots_.SaveHistograms();
    TH2F_run_plots_.SaveHistograms();
}
//_________________________________________________________________________________________
void Coverage::PlotDetectorPhotons(int evt_id) {
    std::stringstream detector_photons_hist_name;
    detector_photons_hist_name << "evt" << std::setfill('0') << std::setw(3) << evt_id  << "_detector_photons";

    TH1F* detector_photons_hist = TH1F_evt_plots_.GetHistogram(detector_photons_hist_name.str());
    if (detector_photons_hist == nullptr) {
        TH1F_evt_plots_.CreateHistogram(detector_photons_hist_name.str(), "Detector Photons", "Entries/bin", 1000, 1, 0);
        detector_photons_hist = TH1F_evt_plots_.GetHistogram(detector_photons_hist_name.str());
    }

    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
	for (const auto& optical_sensor : optical_sensors) {
		int detector_photons = optical_sensor->GetPhotonCount();
        detector_photons_hist->Fill(detector_photons);
	}
    
    TH1F_evt_plots_.SaveHistograms();
}
//_________________________________________________________________________________________
void Coverage::PlotGeometricFraction(const Signal* signal) {
    std::stringstream geometric_fraction_hist_name;
    geometric_fraction_hist_name << "geometric_fraction";

    TH1F* geometric_fraction_hist = TH1F_run_plots_.GetHistogram(geometric_fraction_hist_name.str());
    if (geometric_fraction_hist == nullptr) {
        TH1F_run_plots_.CreateHistogram(geometric_fraction_hist_name.str(), "Geometric Fraction", "Entries/bin", 1000, 1, 0);
        geometric_fraction_hist = TH1F_run_plots_.GetHistogram(geometric_fraction_hist_name.str());
    }

    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
    int detected_photons = 0;
    for (const auto& optical_sensor : optical_sensors) {
        detected_photons += optical_sensor->GetPhotonCount();
    }

    int total_photons = signal->GetScintillation()->GetTotalPhotonCount();
    double geometric_frac = (double)detected_photons / (double)total_photons;

    geometric_fraction_hist->Fill(geometric_frac);
}
//_________________________________________________________________________________________
void Coverage::PlotHistFraction(const Signal* signal) {
    std::stringstream geometric_frac_hist_name;
    geometric_frac_hist_name << "geometric_frac_hist";

    TH2F* geometric_frac_hist = TH2F_run_plots_.GetHistogram(geometric_frac_hist_name.str());
    if (geometric_frac_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(geometric_frac_hist_name.str(), "Intensity Threshold [/(30 cm x 30 cm)]", "Geometric Acceptance", 120, 0, 120, 100, 1, 0);
        geometric_frac_hist = TH2F_run_plots_.GetHistogram(geometric_frac_hist_name.str());
    }

    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
    int total_photons = signal->GetScintillation()->GetTotalPhotonCount();

    std::vector<int> threshold_vector;
    for (int i = 0; i < 100; i += 1) {
        threshold_vector.push_back(i);
        int total_count = 0;
        for (const auto& optical_sensor : optical_sensors) {
            int sensor_count = 0;
            sensor_count += optical_sensor->GetPhotonCount();
            if (sensor_count > i) {
                total_count += sensor_count;
            }
        }
        double frac = (double)total_count/(double)total_photons;
        if (frac > 0) {
            geometric_frac_hist->Fill(i, frac);
        }
    }
}
//_________________________________________________________________________________________
void Coverage::PlotAnodeFraction(const Signal* signal) {
    std::stringstream anode_frac_hist_name;
    anode_frac_hist_name << "anode_frac_hist";

    TH2F* anode_frac_hist = TH2F_run_plots_.GetHistogram(anode_frac_hist_name.str());
    if (anode_frac_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(anode_frac_hist_name.str(), "Anode-plane optical-sensitive coverage", "Geometric acceptance", 100, 1, 0, 100, 1, 0);
        anode_frac_hist = TH2F_run_plots_.GetHistogram(anode_frac_hist_name.str());
    }

    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
    int total_photons = signal->GetScintillation()->GetTotalPhotonCount();
    for (int coverage = 0; coverage < 1000; coverage++) {
        double coverage_frac = (double)coverage / 1000.;

        int photon_count = 0;
        for (const auto& optical_sensor : optical_sensors) {
            photon_count += optical_sensor->GetPhotonCount();
        }

        double collection_frac = coverage_frac * (double)photon_count / (double)total_photons;
        anode_frac_hist->Fill(coverage_frac, collection_frac);
    } 
}
//_________________________________________________________________________________________
/*void Coverage::PlotSNR(const Signal* signal) {
    std::stringstream snr_hist_name;
    snr_hist_name << "snr_hist";

    TH2F* snr_hist = TH2F_run_plots_.GetHistogram(snr_hist_name.str());
    if (snr_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(snr_hist_name.str(), "Uniform noise level [/ 30 cm x 30 cm]", "Sum of ", 800, 0, 800);
        snr_hist = TH2F_run_plots_.GetHistogram(snr_hist_name.str());
    }

    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
    for (int noise_level = 0; noise_level < 1000; noise_level++) {
        double snr_sum = 0.
        for (const auto optical_sensor : optical_sensors) {
            snr_sum += optical_sensor->GetPhotonCount() / noise_level;
        }
        snr_hist->Fill(noise_level, snr_sum);
    }
}*/