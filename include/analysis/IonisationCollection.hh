//____________________________________________________________________________
/*!

\class   analysis::IonisationCollection

\brief   This class performs the ionisation collection analysis.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef IONISATION_COLLECTION_HH
#define IONISATION_COLLECTION_HH

#include <string>
#include <vector>

#include "CLHEP/Random/RandPoisson.h"
#include "CLHEP/Random/RandBinomial.h"

#include "AnalysisResults.hh"
#include "Signal.hh"

class IonisationCollection {
public:
    static AnalysisResults<TH2F> TH2F_run_plots_;

    static void EventAnalysis(const Signal* signal);
    static void RunAnalysis();

    static void PlotThermalElectrons(const Signal* signal);
    static void PlotCollectionElectrons(const Signal* signal);
};

#endif // IONISATION_COLLECTION_HH