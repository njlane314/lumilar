#include "AnalysisManager.hh"

void AnalysisManager::runAnalysis(const Signal* signal) {
    Calorimetry::processSignal(signal);
}

void AnalysisManager::finishAnalysis(std::string outputFilename) {
    Calorimetry::writeToFile(outputFilename);
}