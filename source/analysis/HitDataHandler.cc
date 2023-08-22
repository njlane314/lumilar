#include "HitDataHandler.hh"
//_________________________________________________________________________________________
HitDataHandler* HitDataHandler::instance_ = nullptr;
//_________________________________________________________________________________________
HitDataHandler::HitDataHandler() {}
//_________________________________________________________________________________________
HitDataHandler::~HitDataHandler() {}
//_________________________________________________________________________________________
HitDataHandler* HitDataHandler::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new HitDataHandler();
    }
    return instance_;
}
//_________________________________________________________________________________________
void HitDataHandler::Book() {
    std::string tfile_name = AnalysisManager::GetInstance()->GetOutputFilename();
    std::cout << "-- Opening file: " << tfile_name << std::endl;
    tfile_ = new TFile(tfile_name.c_str(), "UPDATE");


    marley_generator_tree_ = new TTree("marley_generator_tree", "marley generator tree");

    marley_generator_tree_->Branch("marley_generator_neutrino_energy", &marley_generator_neutrino_energy_);
    marley_generator_tree_->Branch("marley_generator_cascade_levels", &marley_generator_cascade_levels_);
    marley_generator_tree_->Branch("marley_generator_decay_type", &marley_generator_decay_type_);

    marley_generator_tree_->Branch("marley_generator_interaction_vertex_x", &marley_generator_interaction_vertex_x_);
    marley_generator_tree_->Branch("marley_generator_interaction_vertex_y", &marley_generator_interaction_vertex_y_);
    marley_generator_tree_->Branch("marley_generator_interaction_vertex_z", &marley_generator_interaction_vertex_z_);

    bxdecay_generator_tree_ = new TTree("bxdecay_generator_tree", "bxdecay generator tree");

    bxdecay_generator_tree_->Branch("bxdecay_generator_momentum", &bxdecay_generator_momentum_);

    event_tree_ = new TTree("event_tree", "event tree");

    event_tree_->Branch("run",   &run_);
    event_tree_->Branch("event", &event_);

    event_tree_->Branch("total_optical_photons",        &total_optical_photons_);
    event_tree_->Branch("total_thermal_electrons",      &total_thermal_electrons_);

    event_tree_->Branch("event_charge_yield",           &event_charge_yield_);
    event_tree_->Branch("event_light_yield",            &event_light_yield_);

    hit_tree_ = new TTree("hit_tree", "hit tree");

    hit_tree_->Branch("hit_energy_deposits",                &hit_energy_deposits_);
    hit_tree_->Branch("hit_step_length",                    &hit_step_length_);
    hit_tree_->Branch("hit_position_x",                     &hit_position_x_);
    hit_tree_->Branch("hit_position_y",                     &hit_position_y_);
    hit_tree_->Branch("hit_position_z",                     &hit_position_z_);
    hit_tree_->Branch("hit_times",                          &hit_times_);
    hit_tree_->Branch("hit_total_energy_deposit",           &hit_total_energy_deposit_);


    arrival_photons_tree_ = new TTree("arrival_photons_tree", "arrival photons tree");

    arrival_photons_tree_->Branch("sensor_arrival_times",    &sensor_arrival_times_);
    arrival_photons_tree_->Branch("sensor_count",            &sensor_count_);
}
//______________________________________________________________________________
void HitDataHandler::Save() {
    tfile_->cd();

    marley_generator_tree_->Write();
    bxdecay_generator_tree_->Write();
    event_tree_->Write();
    hit_tree_->Write();
    arrival_photons_tree_->Write();
    
    tfile_->Close();
}
//______________________________________________________________________________
void HitDataHandler::EventReset() {
    event_ = -1;

    marley_generator_neutrino_energy_ = 0.;
    marley_generator_cascade_levels_.clear();
    marley_generator_decay_type_ = 0;

    marley_generator_interaction_vertex_x_ = 0.;
    marley_generator_interaction_vertex_y_ = 0.;
    marley_generator_interaction_vertex_z_ = 0.;

    bxdecay_generator_momentum_.clear();

    total_optical_photons_ = 0;
    total_thermal_electrons_ = 0;

    event_charge_yield_ = 0;
    event_light_yield_ = 0;

    hit_energy_deposits_.clear();
    hit_step_length_.clear();
    hit_position_x_.clear();
    hit_position_y_.clear();
    hit_position_z_.clear();
    hit_times_.clear();
    hit_total_energy_deposit_ = 0.;

    sensor_arrival_times_.clear();
    sensor_count_.clear();
}
//______________________________________________________________________________
void HitDataHandler::EventFill() {
    event_tree_->Fill();
    marley_generator_tree_->Fill();
    bxdecay_generator_tree_->Fill();
    arrival_photons_tree_->Fill();
    hit_tree_->Fill();
}
//______________________________________________________________________________
void HitDataHandler::SetRun(const int value) {
    run_ = value;
}
//______________________________________________________________________________
void HitDataHandler::SetEvent(const int value) {
    event_ = value;
}
//______________________________________________________________________________
void HitDataHandler::AddMarleyEvent(marley::Event* marley_event, G4ThreeVector* interaction_vertex) {
    marley_generator_neutrino_energy_ = marley_event->projectile().total_energy();

    const auto& marley_cascades = marley_event->get_cascade_levels();
    for (const auto& level : marley_cascades) {
        marley_generator_cascade_levels_.push_back(level->energy());
    }

    marley_generator_decay_type_ = 0;
    bool neutron_decay = false;
    bool proton_decay = false;
    bool alpha_decay = false;
    for (const auto& marley_particle : marley_event->get_final_particles()) {
    	int marley_particle_pdg_code = marley_particle->pdg_code();
        if (marley_particle_pdg_code == 2112) {
	        marley_generator_decay_type_ = 1;
        }
        else if (marley_particle_pdg_code == 2212) {
            marley_generator_decay_type_ = 2;
        }
            else if (marley_particle_pdg_code == 1000020040) {
            marley_generator_decay_type_ = 3;
        }
    }

    marley_generator_interaction_vertex_x_ = interaction_vertex->x();
    marley_generator_interaction_vertex_y_ = interaction_vertex->y();
    marley_generator_interaction_vertex_z_ = interaction_vertex->z();
}
//______________________________________________________________________________
void HitDataHandler::AddBxDecayParticle(const bxdecay0::particle* bxdecay_particle) {
    bxdecay_generator_momentum_.push_back(bxdecay_particle->get_p());
}
//______________________________________________________________________________
void HitDataHandler::AddSignal(Signal* signal) {
    total_optical_photons_ = signal->GetScintillation()->GetTotalPhotonCount();
    total_thermal_electrons_ = signal->GetIonisation()->GetTotalElectronCount();

    std::vector<EnergyDeposit> trajectory_structure = *signal->GetHits();

    for (const auto& hit : trajectory_structure) {
        double energy_deposit = hit.GetEnergy();
        
        hit_energy_deposits_.push_back(energy_deposit);
        hit_step_length_.push_back(hit.GetStepLength());
        
        Eigen::Vector3d position = hit.GetPosition();
        hit_position_x_.push_back(position(0));
        hit_position_y_.push_back(position(1));
        hit_position_z_.push_back(position(2));

        hit_times_.push_back(hit.GetTime());

        hit_total_energy_deposit_ += energy_deposit;
    }   

    event_charge_yield_ = (double) total_thermal_electrons_ / hit_total_energy_deposit_;
    event_light_yield_ = (double) total_optical_photons_ / hit_total_energy_deposit_;

    const OpticalSensorVector& optical_sensors = InstrumentConstruction::GetInstance()->GetOpticalSensors();
    for (const auto& sensor : optical_sensors) {
        sensor_arrival_times_.push_back(sensor->GetArrivalTimes());
        sensor_count_.push_back(sensor->GetPhotonCount());
    }

    this->AddMarleyEvent(&signal->GetMarleyEvent(), &signal->GetMarleyInteractionVertex());
}
