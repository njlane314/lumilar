#include "MarleyGenerator.hh"
//_________________________________________________________________________________________
MarleyGenerator::MarleyGenerator(std::string marley_json, std::string output_filename)
: output_filename_(output_filename) {
    bulk_vertex_generator_ = new BulkVertexGenerator();

    // https://www.sciencedirect.com/science/article/pii/S0090375217300169 
    half_lifes_ = {
        { 0.0298299*MeV, 4.25*ns },
        { 0.800143*MeV, 0.26*ps },
        { 1.64364*MeV, 0.336*us },
        { 1.95907*MeV, 0.54*ps },
        { 2.010368*MeV, 0.32*ps },
    };

    if (!marley_json.empty()) {
        std::cout << "Configuring MARLEY..." << std::endl;
        marley::JSONConfig marley_config(marley_json);
        marley_generator_ = marley_config.create_generator();
    } else {
        std::cout << "MARLEY configuration file not found!  Continuing..."
                  << std::endl;
    }
}
//_________________________________________________________________________________________
MarleyGenerator::~MarleyGenerator() {}
//_________________________________________________________________________________________
void MarleyGenerator::GeneratePrimaryVertex(G4Event* event) {
    bool marley_event_debug = false;

    marley::Event marley_event = marley_generator_.create_event();
    if (marley_event_debug == true) {
        PrintEvent(marley_event);
    }

    double global_time = 0.;
    const int marley_residue_pdg = marley_event.residue().pdg_code();

    int particle_idx = 0;
    int cascade_idx = 0;

    std::vector<G4PrimaryVertex*> primary_vertices;
    G4ThreeVector vertex(0.0, 0.0, 0.0);
    bulk_vertex_generator_->ShootVertex(vertex);  

    const auto& marley_cascades = marley_event.get_cascade_levels();
    for (const auto& marley_particle : marley_event.get_final_particles()) {
        G4PrimaryVertex* primary_vertex = new G4PrimaryVertex(vertex, global_time);
        G4PrimaryParticle* a_particle = new G4PrimaryParticle(marley_particle->pdg_code(), marley_particle->px(), marley_particle->py(), marley_particle->pz(), marley_particle->total_energy());
        a_particle->SetCharge(marley_particle->charge());

        primary_vertex->SetPrimary(a_particle);

        if (particle_idx > 1 && marley_residue_pdg == 1000190400 && cascade_idx < marley_event.get_cascade_levels().size()) {
            const auto& excited_state = marley_cascades[cascade_idx]->energy();

            auto iter = half_lifes_.find(excited_state);
            if (iter != half_lifes_.end()) {
                double time_of_decay = SampleFiniteParticleTime(iter->second);
                global_time += time_of_decay;

                primary_vertex->SetT0(global_time);
            }
            cascade_idx++;
        }

        primary_vertices.push_back(primary_vertex);
        particle_idx++;
    }

    for (const auto& primary_vertex : primary_vertices) {
        event->AddPrimaryVertex(primary_vertex);
    }

    HitDataHandler* hit_data_handler = HitDataHandler::GetInstance();
    hit_data_handler->AddMarleyEvent(&marley_event, &vertex);
}
//_________________________________________________________________________________________
double MarleyGenerator::SampleFiniteParticleTime(double half_life) {
    return CLHEP::RandExponential::shoot(half_life / log(2.));
}
//_________________________________________________________________________________________
void MarleyGenerator::PrintEvent(const marley::Event& event) {
    std::cout << "\nGenerated event data:" << std::endl;
    std::cout << "Projectile energy: " << event.projectile().total_energy() << std::endl;
    std::cout << "Initial excitation energy: " << event.Ex() << " MeV" << std::endl;
    std::cout << "Number of final particles: " << event.get_final_particles().size() << std::endl;
    std::cout << "Number of de-excitation levels: " << event.get_cascade_levels().size() << std::endl;
    std::cout << "Final particles:" << std::endl;
    
    int particle_idx = 1;
    int cascade_idx = 0;
    for (const auto& particle : event.get_final_particles()) {
        std::cout << "Particle " << particle_idx << ":" << std::endl;
        std::cout << "\tPDG code: " << particle->pdg_code() << std::endl;
        std::cout << "\tMomentum (px, py, pz): " << particle->px() << ", " << particle->py() << ", " << particle->pz() << " MeV/c" << std::endl;
        std::cout << "\tTotal energy: " << particle->total_energy() << " MeV" << std::endl;
        std::cout << "\tCharge: " << particle->charge() << std::endl;

        if (particle_idx > 1 && cascade_idx < event.get_cascade_levels().size()) {
        std::cout << "Cascade level: " << cascade_idx << ":" << std::endl;
        std::cout << "\tExcitation energy: " << event.get_cascade_levels()[cascade_idx]->energy() << " MeV" << std::endl;
        const auto& excited_state = event.get_cascade_levels()[cascade_idx]->energy();
        if (excited_state == 1.64364) {
            std::cout << "\tFound a delayed state with energy " << particle->total_energy() << " MeV" << std::endl;
        }
        cascade_idx++;
        }
        
        particle_idx++;
    }
    std::cout << std::endl;
}
