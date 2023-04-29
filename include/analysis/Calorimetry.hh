#ifndef CALORIMETRY_HH
#define CALORIMETRY_HH

#include <string>

#include "Signal.hh"
#include "AnalysisResults.hh"

class Calorimetry {
public:
    static void EventAnalysis(const Signal* signal);
    static void PlotEnergyYields(const Signal* signal);
    static void RunAnalysis();

private:
    static AnalysisResults<TH2F> TH2F_run_plots_;
};

#endif // CALORIMETRY_HH