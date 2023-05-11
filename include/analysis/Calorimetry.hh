//____________________________________________________________________________
/*!

\class   analysis::Calorimetry

\brief   This class performs the calorimetry analysis.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef CALORIMETRY_HH
#define CALORIMETRY_HH

#include <string>

#include "AnalysisResults.hh"
#include "Signal.hh"

class Calorimetry {
public:
    static void EventAnalysis(const Signal* signal);
    static void RunAnalysis();

    static void PlotEnergyYields(const Signal* signal);

private:
    static AnalysisResults<TH2F> TH2F_run_plots_;
};

#endif // CALORIMETRY_HH