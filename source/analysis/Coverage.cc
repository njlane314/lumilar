#include "Coverage.hh"

AnalysisResults<TH1F> Coverage::TH1F_evt_plots_;
AnalysisResults<TH2F> Coverage::TH2F_run_plots_;
AnalysisResults<TH1F> Coverage::TH1F_run_plots_;

void Coverage::EventAnalysis(const Signal* signal) {
    int evt_id = CLHEP::RandFlat::shootInt(1000);
    PlotDetectorPhotons(evt_id);
    PlotGeometricFraction(signal);
    PlotHistFraction(signal);
}

void Coverage::RunAnalysis() {
    TH1F_run_plots_.SaveHistograms();
    TH2F_run_plots_.SaveHistograms();
}

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

void Coverage::PlotHistFraction(const Signal* signal) {
    std::stringstream geometric_frac_hist_name;
    geometric_frac_hist_name << "geometric_frac_hist";

    TH2F* geometric_frac_hist = TH2F_run_plots_.GetHistogram(geometric_frac_hist_name.str());
    if (geometric_frac_hist == nullptr) {
        TH2F_run_plots_.CreateHistogram(geometric_frac_hist_name.str(), "Intensity Threshold [/(30 cm x 30 cm)]", "Geometric Acceptance", 800, 0, 800);
        geometric_frac_hist = TH2F_run_plots_.GetHistogram(geometric_frac_hist_name.str());
    }

    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
    int total_photons = signal->GetScintillation()->GetTotalPhotonCount();

    std::vector<int> threshold_vector;
    for (int i = 0; i < 800; i += 1) {
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