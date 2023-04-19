#ifndef CALORIMETRY_HH
#define CALORIMETRY_HH

#include <string>

#include "Signal.hh"
#include "AnalysisResults.hh"

class Calorimetry {
public:
    static void eventAnalysis(const Signal* signal);
    static void plotEnergyYields(const Signal* signal);
    static void runAnalysis();

private:
    static AnalysisResults<TH2F> TH2F_run_plots_;
};

#endif // CALORIMETRY_HH