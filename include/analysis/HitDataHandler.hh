#ifndef HIT_DATA_HANDLER_HH
#define HIT_DATA_HANDLER_HH

#include "GeneratorParticle.hh"
#include "Particle.hh"
#include "AnalysisManager.hh"

#include "globals.hh"

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

    void SetRun(const int);
    void SetEvent(const int);

    void FillMetadata(const double&, const double&, const double&);

    void AddInitialGeneratorParticle(const GeneratorParticle*);
    void AddFinalGeneratorParticle(const GeneratorParticle*);

    void AddParticle(const Particle*);

    int ProcessToKey(const std::string&);

    inline void AddProcess(const std::string& process) { process_names_.insert(process); }
    inline std::set<std::string> GetProcessNames() const { return process_names_; }

    static HitDataHandler* GetInstance();

private:
    static HitDataHandler* instance_;

    std::set<std::string> process_names_;

    TFile* tfile_;
    TTree* metadata_;
    TTree* event_tree_;

    double detector_length_x_;
    double detector_length_y_;
    double detector_length_z_;

    int run_;
    int event_;

    int number_particles_;
    int number_hits_;

    double energy_deposit_;

    std::vector<int> particle_track_id_;
    std::vector<int> particle_parent_track_id_;
    std::vector<int> particle_pdg_code_;
    std::vector<double> particle_mass_;
    std::vector<double> particle_charge_;
    std::vector<int> particle_process_key_;
    std::vector<int> particle_total_occupancy_;

    std::vector<int> particle_number_daughters_;
    std::vector<std::vector<int>> particle_daughter_track_ids_;

    std::vector<double> particle_initial_x_;
    std::vector<double> particle_initial_y_;
    std::vector<double> particle_initial_z_;
    std::vector<double> particle_initial_t_;

    std::vector<double> particle_initial_px_;
    std::vector<double> particle_initial_py_;
    std::vector<double> particle_initial_pz_;
    std::vector<double> particle_initial_energy_;

    std::vector<int> hit_track_id_;
    std::vector<double> hit_start_x_;
    std::vector<double> hit_start_y_;
    std::vector<double> hit_start_z_;
    std::vector<double> hit_start_t_;
    std::vector<double> hit_end_x_;
    std::vector<double> hit_end_y_;
    std::vector<double> hit_end_z_;
    std::vector<double> hit_end_t_;
    std::vector<double> hit_length_;
    std::vector<double> hit_energy_deposit_;
    std::vector<int> hit_process_key_;

    int generator_initial_number_particles_;
    int generator_final_number_particles_;

    std::vector<double> generator_initial_particle_x_;
    std::vector<double> generator_initial_particle_y_;
    std::vector<double> generator_initial_particle_z_;
    std::vector<double> generator_initial_particle_t_;
    std::vector<double> generator_initial_particle_px_;
    std::vector<double> generator_initial_particle_py_;
    std::vector<double> generator_initial_particle_pz_;
    std::vector<double> generator_initial_particle_energy_;
    std::vector<int> generator_initial_particle_pdg_code_;
    std::vector<double> generator_initial_particle_mass_;
    std::vector<double> generator_initial_particle_charge_;

    std::vector<double> generator_final_particle_x_;
    std::vector<double> generator_final_particle_y_;
    std::vector<double> generator_final_particle_z_;
    std::vector<double> generator_final_particle_t_;
    std::vector<double> generator_final_particle_px_;
    std::vector<double> generator_final_particle_py_;
    std::vector<double> generator_final_particle_pz_;
    std::vector<double> generator_final_particle_energy_;
    std::vector<int> generator_final_particle_pdg_code_;
    std::vector<double> generator_final_particle_mass_;
    std::vector<double> generator_final_particle_charge_;
};

#endif // HIT_DATA_HANDLER_HH