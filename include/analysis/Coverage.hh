//____________________________________________________________________________
/*!

\class   analysis::Coverage

\brief   This class performs the coverage analysis.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef COVERAGE_HH
#define COVERAGE_HH

#include <string>

#include "AnalysisResults.hh"
#include "Signal.hh"
#include "InstrumentConstruction.hh"

class Coverage {
public:
    static void EventAnalysis(const Signal* signal);
    static void RunAnalysis();

    static void PlotDetectorPhotons(int evt_id);
    static void PlotGeometricFraction(const Signal* signal);
    static void PlotHistFraction(const Signal* signal);
    static void PlotAnodeFraction(const Signal* signal);

private:
    static AnalysisResults<TH1F> TH1F_evt_plots_;
    static AnalysisResults<TH2F> TH2F_run_plots_;
    static AnalysisResults<TH1F> TH1F_run_plots_;
};

#endif // COVERAGE_HH