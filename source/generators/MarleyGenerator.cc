#include "MarleyGenerator.hh"
//_________________________________________________________________________________________
AnalysisResults<TH1F> MarleyGenerator::TH1F_plots_;
//_________________________________________________________________________________________
MarleyGenerator::MarleyGenerator(std::string marley_source, std::string output_filename)
: source_(marley_source), output_filename_(output_filename) {
    bulk_vertex_generator_ = new BulkVertexGenerator();

    // https://www.sciencedirect.com/science/article/pii/S0090375217300169 
    half_lifes_ = {
        //{ 0.0298299*MeV, 4.25*ns },
        //{ 0.800143*MeV, 0.26*ps },
        { 1.64364*MeV, 0.336*us },
        //{ 1.95907*MeV, 0.54*ps },
        //{ 2.010368*MeV, 0.32*ps },
    };

    //energy_dist_name_ = "neutrino_energy_dist";
    //time_dist_name_ = "cascade_time_dist";

    //TH1F_plots_.CreateHistogram(energy_dist_name_, "Neutrino Energy [MeV]", "Events/bin", 20, 0, 20);
    //TH1F_plots_.CreateHistogram(time_dist_name_, "Cascade Time [ns]", "Entries/bin", 100, 1, 0);
}
//_________________________________________________________________________________________
MarleyGenerator::~MarleyGenerator() {}
//_________________________________________________________________________________________
void MarleyGenerator::GeneratePrimaryVertex(G4Event* event) {
    marley::JSONConfig marley_config(source_);
    marley::Generator marley_generator = marley_config.create_generator();
    marley::Event marley_event = marley_generator.create_event();

    auto primary_energy = marley_event.projectile().total_energy();
    Signal::GetInstance()->RecordPrimaryEnergy(primary_energy);
    
    //TH1F* energy_hist = TH1F_plots_.GetHistogram(energy_dist_name_);
    //energy_hist->Fill(primary_energy);

    double global_time = 0.;

    const auto& marley_cascades = marley_event.get_cascade_levels();
    const int marley_residue_pdg = marley_event.residue().pdg_code();

    int particle_idx = 0;
    int cascade_idx = 0;

    std::vector<G4PrimaryVertex*> primary_vertices;
    G4ThreeVector vertex(0.0, 0.0, 0.0);
    bulk_vertex_generator_->ShootVertex(vertex);  

    for (const auto& marley_particle : marley_event.get_final_particles()) {
        G4PrimaryVertex* primary_vertex = new G4PrimaryVertex(vertex, global_time);
        G4PrimaryParticle* a_particle = new G4PrimaryParticle(marley_particle->pdg_code(), marley_particle->px(), marley_particle->py(), marley_particle->pz(), marley_particle->total_energy());
        a_particle->SetCharge(marley_particle->charge());

        primary_vertex->SetPrimary(a_particle);

        if (particle_idx > 1 && marley_residue_pdg == 1000190400 && cascade_idx < marley_event.get_cascade_levels().size()) {
            const auto& excited_state = marley_cascades[cascade_idx]->energy();
            
            /*std::string cascade_level_hist_name = "cascade_level_" + std::to_string(excited_state) + "_hist";

            TH1F* cascade_level_hist = TH1F_plots_.GetHistogram(cascade_level_hist_name);

            if (cascade_level_hist == nullptr) {
                TH1F_plots_.CreateHistogram(cascade_level_hist_name, "Neutrino energy [MeV]", "Cascade levels/MeV", 20, 0, 20);
                cascade_level_hist = TH1F_plots_.GetHistogram(cascade_level_hist_name);
            }

            cascade_level_hist->Fill(primary_energy);*/

            auto iter = half_lifes_.find(excited_state);
            if (iter != half_lifes_.end()) {
                double time_of_decay = SampleFiniteParticleTime(iter->second);
                global_time += time_of_decay;
                primary_vertex->SetT0(global_time);
                //TH1F* time_hist = TH1F_plots_.GetHistogram(time_dist_name_);
                //time_hist->Fill(time_of_decay);
                Signal::GetInstance()->RecordDelayTime(time_of_decay);
            }
            cascade_idx++;
        }

        primary_vertices.push_back(primary_vertex);
        particle_idx++;
    }

    for (const auto& primary_vertex : primary_vertices) {
        event->AddPrimaryVertex(primary_vertex);
    }

    int event_idx = event->GetEventID();
    int events_to_generate = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

    /*(if (event_idx == events_to_generate - 1) {
        TH1F_plots_.WriteToFile(output_filename_);
        TH1F_plots_.StackHistograms("cascade_level", "Neutrino Energy [MeV]", "Cascade Levels/MeV");
    }*/
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