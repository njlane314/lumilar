#ifndef COVERAGE_HH
#define COVERAGE_HH

#include <string>

#include "Signal.hh"
#include "InstrumentConstruction.hh"
#include "AnalysisResults.hh"

class Coverage {
public:
    static void EventAnalysis(const Signal* signal);
    static void RunAnalysis();

    static void PlotDetectorPhotons(int evt_id);
    static void PlotGeometricFraction(const Signal* signal);
    static void PlotHistFraction(const Signal* signal);

private:
    static AnalysisResults<TH1F> TH1F_evt_plots_;
    static AnalysisResults<TH2F> TH2F_run_plots_;
    static AnalysisResults<TH1F> TH1F_run_plots_;
};

#endif // COVERAGE_HH