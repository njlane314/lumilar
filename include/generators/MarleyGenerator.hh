//____________________________________________________________________________
/*!

\class   generators::MarleyGenerator

\brief   This class generates events using the Marley neutrino generator, and
        accounts for finite particle lifetimes.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef MARLEY_GENERATOR_HH
#define MARLEY_GENERATOR_HH

#include <string>
#include <iostream>

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "marley/Event.hh"
#include "marley/Generator.hh"
#include "marley/JSONConfig.hh"

#include "Signal.hh"
#include "AnalysisResults.hh"

#include <CLHEP/Random/RandExponential.h>

#include "TFile.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TMultiGraph.h"
#include "TH2F.h"

class MarleyGenerator {
public:
    MarleyGenerator(std::string marley_source, std::string output_filename);
    ~MarleyGenerator();

    std::map<double, double> half_lifes_;

    void GeneratePrimaryVertex(G4Event* event);
    double SampleFiniteParticleTime(double half_life);
    
    void PrintEvent(const marley::Event& event);

private:
    std::string source_;
    std::string output_filename_;
    TH1F* energy_hist_;
    TH1F* delayed_hist_;
    TH1F* delay_times_;
    static AnalysisResults<TH1F> TH1F_plots_;
    std::string energy_dist_name_;
    std::string time_dist_name_;
};

#endif // MARLEY_GENERATOR_HH
