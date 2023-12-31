#ifndef HIT_DATA_HANDLER_HH
#define HIT_DATA_HANDLER_HH

#include "AnalysisManager.hh"
#include "Signal.hh"
#include "OpticalSensor.hh"
#include "InstrumentConstruction.hh"

#include "marley/Event.hh"

#include "globals.hh"

#include "G4ThreeVector.hh"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include "BxDecay0Generator.hh"
#include "bxdecay0/particle.h"  

#include <map>
#include <set>

class HitDataHandler {
public:
    HitDataHandler();
    ~HitDataHandler();

    void Book();
    void Save();
    void EventFill();
    void EventReset();

    void SetRun(const int);
    void SetEvent(const int);

    void AddSignal(Signal* signal);
    void AddBxDecayParticle(const bxdecay0::particle* bxdecay_particle);
    void AddMarleyEvent(marley::Event*, G4ThreeVector*);

    static HitDataHandler* GetInstance();

private:
    static HitDataHandler* instance_;

    TFile* tfile_;
    
    TTree* event_tree_;
    TTree* hit_tree_;
    TTree* marley_generator_tree_;
    TTree* bxdecay_generator_tree_;
    TTree* arrival_photons_tree_;

    int run_;
    int event_;

    double marley_generator_neutrino_energy_;
    std::vector<double> marley_generator_cascade_levels_;
    int marley_generator_decay_type_;

    double marley_generator_interaction_vertex_x_;
    double marley_generator_interaction_vertex_y_;
    double marley_generator_interaction_vertex_z_;

    std::vector<double> bxdecay_generator_momentum_;

    int total_optical_photons_;
    int total_thermal_electrons_;

    double event_charge_yield_;
    double event_light_yield_;

    std::vector<double> hit_energy_deposits_;
    std::vector<double> hit_step_length_;
    std::vector<double> hit_position_x_;
    std::vector<double> hit_position_y_;
    std::vector<double> hit_position_z_;
    std::vector<double> hit_times_;
    double hit_total_energy_deposit_;
    
    std::vector<std::vector<double>> sensor_arrival_times_;
    std::vector<int> sensor_count_;
    double sensor_neutrino_energy_;
};

#endif // HIT_DATA_HANDLER_HH
