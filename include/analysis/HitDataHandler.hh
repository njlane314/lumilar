#ifndef HIT_DATA_HANDLER_HH
#define HIT_DATA_HANDLER_HH

#include "GeneratorParticle.hh"
#include "Particle.hh"
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

    void AddMarleyEvent(const marley::Event* event, const G4ThreeVector* interaction_vertex);
    void AddSignal(const Signal* signal);

    static HitDataHandler* GetInstance();

private:
    static HitDataHandler* instance_;

    TFile* tfile_;

    TTree* marley_generator_tree_;
    TTree* event_tree_;
    TTree* hit_tree_;
    TTree* arrival_photons_tree_;

    double marley_generator_neutrino_energy_;
    std::vector<double> marley_generator_cascade_levels_;
    int marley_generator_decay_type_;

    double marley_generator_interaction_vertex_x_;
    double marley_generator_interaction_vertex_y_;
    double marley_generator_interaction_vertex_z_;

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
};

#endif // HIT_DATA_HANDLER_HH