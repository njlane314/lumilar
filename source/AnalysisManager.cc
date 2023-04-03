#include "AnalysisManager.hh"

AnalysisManager* AnalysisManager::instance_ = nullptr;

AnalysisManager::AnalysisManager() {}

AnalysisManager::~AnalysisManager() {}

AnalysisManager* AnalysisManager::Instance() {
    if (!instance_) {
        instance_ = new AnalysisManager();
    }
    
    return instance_;
}

void AnalysisManager::SaveHistograms() {
    std::cout << "Saving histograms..." << std::endl;
    TFile file("analysis.root", "RECREATE");
    std::map<std::string, TH1F*>::iterator it;
    for (it = TH1F_map_.begin(); it != TH1F_map_.end(); it++) {
        std::cout << "Writing histogram: " << it->first << std::endl;
        it->second->Write();
    }
    std::map<std::string, TH2F*>::iterator it2D;
    for (it2D = TH2F_map_.begin(); it2D != TH2F_map_.end(); it2D++) {
        std::cout << "Writing histogram: " << it2D->first << std::endl;
        it2D->second->Write();
    }

    file.Close();
}

TH1F* AnalysisManager::CreateHistogram(const std::string& name, const std::string& x_axis_name, const std::string& y_axis_name, const int nbinsx, const double xmin, const double xmax) {
    TH1F* histogram = new TH1F(name.c_str(), "", nbinsx, xmin, xmax);
    histogram->GetXaxis()->SetTitle(x_axis_name.c_str());
    histogram->GetYaxis()->SetTitle(y_axis_name.c_str());
    TH1F_map_[name] = histogram;
    return histogram;
}

TH2F* AnalysisManager::CreateHistogram(const std::string& name, const std::string& x_axis_name, const std::string& y_axis_name, const int nbinsx, const double xmin, const double xmax, const int nbinsy, const double ymin, const double ymax) {
    TH2F* histogram = new TH2F(name.c_str(), "", nbinsx, xmin, xmax, nbinsy, ymin, ymax);
    histogram->GetXaxis()->SetTitle(x_axis_name.c_str());
    histogram->GetYaxis()->SetTitle(y_axis_name.c_str());
    TH2F_map_[name] = histogram;
    return histogram;
}

TH2F* AnalysisManager::GetHistogram(std::map<std::string, TH2F*>& hist_map, const std::string& name, const std::string& x_axis_name /*= "X Axis"*/, const std::string& y_axis_name /*= "Y Axis"*/, const int nbinsx /*= 100*/, const double xmin /*= 1*/, const double xmax /*= 0*/, const int nbinsy /*= 100*/, const double ymin /*= 1*/, const double ymax /*= 0*/) {
    TH2F* histogram = nullptr;
    auto it = hist_map.find(name);
    if (it != hist_map.end()) {
        histogram = it->second;
    } else {
        histogram = CreateHistogram(name, x_axis_name, y_axis_name, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
        hist_map[name] = histogram;
    }
    return histogram;
}

TH1F* AnalysisManager::GetHistogram(std::map<std::string, TH1F*>& hist_map, const std::string& name, const std::string& x_axis_name /*= "X Axis"*/, const std::string& y_axis_name /*= "Y Axis"*/, const int nbinsx /*= 100*/, const double xmin /*= 1*/, const double xmax /*= 0*/) {
    TH1F* histogram = nullptr;
    auto it = hist_map.find(name);
    if (it != hist_map.end()) {
        histogram = it->second;
    } else {
        histogram = CreateHistogram(name, x_axis_name, y_axis_name, nbinsx, xmin, xmax);
        hist_map[name] = histogram;
    }
    return histogram;
}

void AnalysisManager::DiscreteResponse(const Scintillation* scintillation, const Ionisation* ionisation) {
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();

    charge_step_response = GetHistogram(TH1F_map_, "charge_step_response", "Charge Step Response", "Entries/bin");
    light_step_response = GetHistogram(TH1F_map_, "light_step_response", "Light Step Response", "Entries/bin");
    joint_step_response = GetHistogram(TH1F_map_, "joint_step_response", "Joint Step Response", "Entries/bin");

    if (radiant_sizes.size() == cloud_sizes.size()) {
        for (int i = 0; i < radiant_sizes.size(); i++) {
            charge_step_response->Fill(cloud_sizes[i]);
            light_step_response->Fill(radiant_sizes[i]);
            joint_step_response->Fill(radiant_sizes[i] + cloud_sizes[i]);
        }
    }
}

void AnalysisManager::EventResponse(const G4Event* event, const Signal* signal, const Scintillation* scintillation, const Ionisation* ionisation) {
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();

    std::vector<double> visible_deposits = signal->get_visible_deposits();
    double energy = event->GetPrimaryVertex()->GetPrimary()->GetTotalEnergy();

    charge_event_response = GetHistogram(TH1F_map_, "charge_event_response", "Charge Event Response", "Entries/bin");
    light_event_response = GetHistogram(TH1F_map_, "light_event_response", "Light Event Repsonse", "Entries/bin");
    joint_event_response = GetHistogram(TH1F_map_, "joint_event_response", "Joint Event Response", "Entries/bin");

    charge_event_response_energy = GetHistogram(TH2F_map_, "charge_event_response_energy", "Initial Primary Energy [MeV]", "Charge Event Response/bin");
    light_event_response_energy = GetHistogram(TH2F_map_, "light_event_response_energy", "Initial Primary Energy [MeV]", "Light Event Response/bin");
    joint_event_response_energy = GetHistogram(TH2F_map_, "joint_event_response_energy", "Initial Primary Energy [MeV]", "Joint Event Response/bin");

    charge_event_visible_energy = GetHistogram(TH2F_map_, "charge_event_visible_energy", "Visible Energy [MeV]", "Charge Event Response/bin");
    light_event_visible_energy = GetHistogram(TH2F_map_, "light_event_visible_energy", "Visible Primary Energy [MeV]", "Light Event Response/bin");
    joint_event_visible_energy = GetHistogram(TH2F_map_, "joint_event_visible_energy", "Visible Primary Energy [MeV]", "Joint Event Response/bin");

    int photon_count = 0;
    int electron_count = 0;
    double visible_deposit = 0;
    if (radiant_sizes.size() == cloud_sizes.size()) {
        for (int i = 0; i < radiant_sizes.size(); i++) {
            photon_count += radiant_sizes[i];
            electron_count += cloud_sizes[i];
            visible_deposit += visible_deposits[i];
        }
    }

    charge_event_response->Fill(electron_count);
    light_event_response->Fill(photon_count);
    joint_event_response->Fill(photon_count + electron_count);

    charge_event_response_energy->Fill(energy, electron_count);
    light_event_response_energy->Fill(energy, photon_count);
    joint_event_response_energy->Fill(energy, photon_count + electron_count);

    charge_event_visible_energy->Fill(visible_deposit, electron_count);
    light_event_visible_energy->Fill(visible_deposit, photon_count);
    joint_event_visible_energy->Fill(visible_deposit, photon_count + electron_count);
}

void AnalysisManager::SignalYield(const Signal* signal, const Scintillation* scintillation, const Ionisation* ionisation) {
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();

    std::vector<double> visible_deposits = signal->get_visible_deposits();
    std::vector<double> linear_transfers = signal->get_linear_transfers();

    charge_event_yield = GetHistogram(TH1F_map_, "charge_event_yield", "Charge Event Yield [/MeV]", "Entries/bin");
    light_event_yield = GetHistogram(TH1F_map_, "light_event_yield", "Light Event Yield [/MeV]", "Entries/bin");
    joint_event_yield = GetHistogram(TH1F_map_, "joint_event_yield", "Joint Event Yield [/MeV]", "Entries/bin");

    charge_step_yield = GetHistogram(TH2F_map_, "charge_step_yield", "Charge Step Yield [/MeV]", "Entries/bin");
    light_step_yield = GetHistogram(TH2F_map_, "light_step_yield", "Light Step Yield [/MeV]", "Entries/bin");
    joint_step_yield = GetHistogram(TH2F_map_, "joint_step_yield", "Joint Step Yield [/MeV]", "Entries/bin");

    charge_step_density = GetHistogram(TH2F_map_, "charge_length_yield", "Charge Step Density [/cm]", "Entries/bin");
    light_step_density = GetHistogram(TH2F_map_, "light_length_yield", "Light Step Density [/cm]", "Entries/bin");
    joint_step_density = GetHistogram(TH2F_map_, "joint_length_yield", "Joint Step Density [/cm]", "Entries/bin");

    int photon_count = 0;
    int electron_count = 0;
    double visible_deposit = 0;
    if (radiant_sizes.size() == cloud_sizes.size()) {
        for (int i = 0; i < radiant_sizes.size(); i++) {
            photon_count += radiant_sizes[i];
            electron_count += cloud_sizes[i];
            visible_deposit += visible_deposits[i];

            charge_step_yield->Fill(linear_transfers[i], cloud_sizes[i] / visible_deposits[i]);
            light_step_yield->Fill(linear_transfers[i], radiant_sizes[i] / visible_deposits[i]);
            joint_step_yield->Fill(linear_transfers[i], (cloud_sizes[i] + radiant_sizes[i]) / visible_deposits[i]);

            charge_step_density->Fill(linear_transfers[i], cloud_sizes[i] / (linear_transfers[i] / visible_deposits[i]));
            light_step_density->Fill(linear_transfers[i], radiant_sizes[i] / (linear_transfers[i] / visible_deposits[i]));
            joint_step_density->Fill(linear_transfers[i], (cloud_sizes[i] + radiant_sizes[i]) / (linear_transfers[i] / visible_deposits[i]));
        }
    }

    charge_event_yield->Fill(electron_count / visible_deposit);
    light_event_yield->Fill(photon_count / visible_deposit);
    joint_event_yield->Fill((photon_count + electron_count) / visible_deposit);
}

void AnalysisManager::StackPulseShape(const Scintillation* scintillation) {
    std::vector<double> emission_times = scintillation->get_emission_times();

    int xmin = 0;
    int xmax = 2000;
    double time_resolution = 1;

    double bins = (xmax - xmin) / time_resolution;

    pulse_shape = GetHistogram(TH1F_map_, "pulse_shape", "Emission time [ns]", "Entries/bin", bins, xmin, xmax);

    for (int i = 0; i < emission_times.size(); i++) {
        pulse_shape->Fill(emission_times[i]);
    }
}

void AnalysisManager::RandomPulseShape(const Scintillation* scintillation) {
    std::vector<double> emission_times = scintillation->get_emission_times();

    int xmin = 0;
    int xmax = 2000;
    double time_resolution = 1;

    int bins = round((double)(xmax - xmin) / (double)time_resolution);
    random_pulse_shape = GetHistogram(TH1F_map_, "random_pulse_shape", "Emission time [ns]", "Entries/bin", bins, xmin, xmax);
    random_pulse_shape->Reset();
    for (int i = 0; i < emission_times.size(); i++) {
        random_pulse_shape->Fill(emission_times[i]);
    }
}

void AnalysisManager::PulseShapeDiscrimination(const Scintillation* scintillation, const Ionisation* ionisation) {
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();
    std::vector<double> emission_times = scintillation->get_emission_times();

    int prompt_window = 200;

    prompt_fraction_scint = GetHistogram(TH2F_map_, "prompt_fraction", "Scintillation count", "Prompt fraction");
    prompt_fraction_ratio = GetHistogram(TH2F_map_, "prompt_fraction_ratio", "S1/S2", "Prompt fraction");

    int photon_count = 0;
    int electron_count = 0;
    if (radiant_sizes.size() == cloud_sizes.size()) {
        for (int i = 0; i < radiant_sizes.size(); i++) {
            photon_count += radiant_sizes[i];
            electron_count += cloud_sizes[i];
        }
    }

    int prompt_count = 0;
    for (int i = 0; i < emission_times.size(); i++) {
        if (emission_times[i] < prompt_window) {
            prompt_count++;
        }
    }
    double f_prompt = (double) prompt_count / (double) emission_times.size();

    prompt_fraction_scint->Fill(photon_count, f_prompt);
    prompt_fraction_ratio->Fill((double)photon_count / (double)electron_count, f_prompt);
}