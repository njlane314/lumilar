//____________________________________________________________________________
/*!

\class   analysis::ScintillationCollection

\brief   This class performs the scintillation collection analysis.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef SCINTILLATION_COLLECTION_HH
#define SCINTILLATION_COLLECTION_HH

#include <string>
#include <vector>

#include "CLHEP/Random/RandPoisson.h"

#include "AnalysisResults.hh"
#include "Signal.hh"

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
