#ifndef MARLEY_GENERATOR_HH
#define MARLEY_GENERATOR_HH

#include <string>
#include <iostream>

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "marley/Event.hh"
#include "marley/Generator.hh"
#include "marley/JSONConfig.hh"

#include "marley/marley_utils.hh"
#include "marley/DecayScheme.hh"
#include "marley/MassTable.hh"
#include "marley/Level.hh"

#include "DetectorConstruction.hh"

class MarleyGenerator {
public:
    MarleyGenerator(std::string marley_source);
    ~MarleyGenerator();

    std::map<double, double> cascade_times_;

    void GeneratePrimaryVertex(G4Event* event);
    double SampleDecayTime(double half_life);
    
    void PrintEvent(const marley::Event& event);

private:
    std::string source_;
    int delay_states_;

    std::vector<std::pair<double, double>> halflife_vec_;
};

#endif // MARLEY_GENERATOR_HH
