//____________________________________________________________________________
/*!

\class   analysis::Pulseshape

\brief   This class performs the pulseshape analysis.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef PULSESHAPE_HH
#define PULSESHAPE_HH

#include <string>
#include <vector>

#include "CLHEP/Random/RandFlat.h"

#include "AnalysisResults.hh"
#include "Signal.hh"
#include "InstrumentConstruction.hh"
#include "OpticalSensor.hh"

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
