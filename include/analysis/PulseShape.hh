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
    static AnalysisResults<TH1F> results_;
    static AnalysisResults<TProfile> auto_correlation_;
    static AnalysisResults<TH2F> run_analysis_;

    static void processSignal(const Signal* signal);
    static void writeToFile(const std::string& filename);
    static void writeAndFinish(const std::string& filename);
};

#endif // PULSESHAPE_HH
