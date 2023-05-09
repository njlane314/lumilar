#ifndef PULSESHAPE_HH
#define PULSESHAPE_HH

#include "AnalysisResults.hh"
#include "OpticalSensor.hh"
#include "InstrumentConstruction.hh"
#include "Signal.hh"

#include "CLHEP/Random/RandFlat.h"
#include <string>
#include <vector>

#include "TSpectrum.h"

class PulseShape {
public:
    static AnalysisResults<TH1F> TH1F_evt_plots_;
    static AnalysisResults<TProfile> TProfile_evt_plots_;
    static AnalysisResults<TH2F> TH2F_run_plots_;

    static void EventAnalysis(const Signal* signal);
    static void RunAnalysis();
    static void PlotEmissionTimes(const Signal* signal, int evt_id);
    static void PlotAmplitudeRatio(const Signal* signal, int evt_id);
    static void PlotArrivalTimes(int evt_id);
    static void PlotWavelengths(const Signal* signal, int evt_id);
};

#endif // PULSESHAPE_HH
