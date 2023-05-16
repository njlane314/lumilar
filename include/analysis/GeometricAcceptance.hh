//____________________________________________________________________________
/*!

\class   analysis::GeometricAcceptance

\brief   This class performs the GeometricAcceptance analysis.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef GEOMETRIC_ACCEPTANCE_HH
#define GEOMETRIC_ACCEPTANCE_HH

#include <string>

#include "AnalysisResults.hh"
#include "Signal.hh"
#include "InstrumentConstruction.hh"

class GeometricAcceptance {
public:
    static void EventAnalysis(const Signal* signal);
    static void RunAnalysis();

    static void PlotGeometricAcceptanceDistance(const Signal* signal);

private:
    static AnalysisResults<TH2F> TH2F_run_plots_;
};

#endif // GEOMETRIC_ACCEPTANCE_HH