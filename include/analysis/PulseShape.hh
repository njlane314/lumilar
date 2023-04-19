#ifndef PULSESHAPE_HH
#define PULSESHAPE_HH

#include "AnalysisResults.hh"
#include "OpticalSensor.hh"
#include "CLHEP/Random/RandFlat.h"
#include <string>
#include <vector>
#include "Signal.hh"
#include "TSpectrum.h"

class PulseShape {
public:
    static AnalysisResults<TH1F> TH1F_evt_plots_;
    static AnalysisResults<TH2F> TProfile_evt_plots_;

    static void eventAnalysis(const Signal* signal);
    static void plotEmissionTimes(const Signal* signal);
};

#endif // PULSESHAPE_HH
