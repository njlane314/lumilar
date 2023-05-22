#ifndef TRUTH_MANAGER_HH
#define TRUTH_MANAGER_HH

#include "GeneratorParticle.hh"
#include "Particle.hh"

#include "globals.hh"

#include "marley/Particle.hh"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include <map>
#include <set>

class TruthManager {
public:

    TruthManager();
    ~TruthManager();

    void EventReset();

    void SetRun(const int);
    void SetEvent(const int);

    static TruthManager* GetInstance();

    void AddInitialGeneratorParticle(GeneratorParticle*);
    void AddFinalGeneratorParticle(GeneratorParticle*);

    void AddInitialMarleyParticle(const marley::Particle&);
    void AddFinalMarleyParticle(const marley::Particle&);

    void AddParticle(Particle*);
    Particle* GetParticle(const int);

    inline std::vector<GeneratorParticle*> GetInitialGeneratorParticles() const { return initial_generator_particles_; }
    inline std::vector<GeneratorParticle*> GetFinalGeneratorParticles() const { return final_generator_particles_; }

    inline std::vector<marley::Particle> GetInitialMarleyParticles() const { return initial_marley_particles_; }
    inline std::vector<marley::Particle> GetFinalMarleyParticles() const { return final_marley_particles_; }

    inline std::map<int, Particle*> GetParticleMap() const { return particle_map_; }

private:

    static TruthManager * instance_;

    int run_;
    int event_;

    std::vector<GeneratorParticle*> initial_generator_particles_;
    std::vector<GeneratorParticle*> final_generator_particles_;

    std::vector<marley::Particle> initial_marley_particles_;
    std::vector<marley::Particle> final_marley_particles_;

    std::map<int, Particle*> particle_map_;
};

#endif // TRUTH_MANAGER_HH