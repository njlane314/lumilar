#ifndef SCINTILLATION_COLLECTION_HH
#define SCINTILLATION_COLLECTION_HH

#include "AnalysisResults.hh"
#include "Signal.hh"

#include "CLHEP/Random/RandPoisson.h"
#include <string>
#include <vector>

class ScintillationCollection {
public:
    static AnalysisResults<TH2F> TH2F_run_plots_;

    static void EventAnalysis(const Signal* signal);
    static void RunAnalysis();

    static void PlotEmissionPhotons(const Signal* signal);
    static void PlotAmplitudeRatio(const Signal* signal);
    static void PlotCollectionFraction(const Signal* signal);
};

#endif // SCINTILLATION_COLLECTION_HH
