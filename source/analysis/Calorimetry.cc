#include "Calorimetry.hh"

AnalysisResults<TH1F> Calorimetry::results_;
int signal_idx = 0;

void Calorimetry::processSignal(const Signal* signal) {
    signal_idx++;
    results_.fillHistogram(std::to_string(signal_idx), "eventPhotons", signal->get_scintillation()->get_total_photons());
}

void Calorimetry::writeToFile(const std::string& filename) {
    results_.writeToFile(filename);
    results_.clear();
}
