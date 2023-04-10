#ifndef ANALYSIS_MANAGER_HH
#define ANALYSIS_MANAGER_HH

#include "Signal.hh"
#include "Calorimetry.hh"

class AnalysisManager {
public:
    void runAnalysis(const Signal* signal);
    void finishAnalysis(std::string outputFilename);
};

#endif // ANALYSIS_MANAGER_HH