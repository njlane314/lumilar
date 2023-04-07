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

#include <CLHEP/Random/RandExponential.h>

class MarleyGenerator {
public:
    MarleyGenerator(std::string marley_source);
    ~MarleyGenerator();

    std::map<double, double> half_lifes_;

    void GeneratePrimaryVertex(G4Event* event);
    double sample_decay_time(double half_life);
    
    void print_event(const marley::Event& event);

private:
    std::string source_;
    int delay_states_;
};

#endif // MARLEY_GENERATOR_HH
