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

void AnalysisManager::AnalyseSignalResponse(const G4Event* event, const Scintillation* scintillation, const Ionisation* ionisation) {
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();

    hist_charge = GetHistogram(TH1F_map_, "hist_charge", "Charge", "Counts");
    hist_light = GetHistogram(TH1F_map_, "hist_light", "Light", "Counts");
    hist_quanta = GetHistogram(TH1F_map_, "hist_quanta", "Quanta", "Counts");

    if (radiant_sizes.size() == cloud_sizes.size()) {
        for (int i = 0; i < radiant_sizes.size(); i++) {
            hist_light->Fill(radiant_sizes[i]);
            hist_charge->Fill(cloud_sizes[i]);
            hist_quanta->Fill(radiant_sizes[i] + cloud_sizes[i]);
        }
    }
}

void AnalysisManager::AnalyseVariableEnergySignal(const G4Event* event, const Scintillation* scintillation, const Ionisation* ionisation) {
    double energy = event->GetPrimaryVertex()->GetPrimary()->GetTotalEnergy();
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();

    hist_charge_energy = GetHistogram(TH2F_map_, "hist_charge_energy", "Energy", "Charge");
    hist_light_energy = GetHistogram(TH2F_map_, "hist_light_energy", "Energy", "Light");
    hist_quanta_energy = GetHistogram(TH2F_map_, "hist_quanta_energy", "Energy", "Quanta");

    int photon_count = 0;
    int electron_count = 0;
    if (radiant_sizes.size() == cloud_sizes.size()) {
        for (int i = 0; i < radiant_sizes.size(); i++) {
            photon_count += radiant_sizes[i];
            electron_count += cloud_sizes[i];
        }
    }

    hist_light_energy->Fill(energy, photon_count);
    hist_charge_energy->Fill(energy, electron_count);
    hist_quanta_energy->Fill(energy, photon_count + electron_count);
}

void AnalysisManager::AnalyseFixedEnergySignal(const G4Event* event, const Scintillation* scintillation, const Ionisation* ionisation) {
    double energy = event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();

    hist_charge_fixed = GetHistogram(TH1F_map_, "hist_charge_fixed", "Charge", "Counts");
    hist_light_fixed = GetHistogram(TH1F_map_, "hist_light_fixed", "Light", "Counts");
    hist_quanta_fixed = GetHistogram(TH1F_map_, "hist_quanta_fixed", "Quanta", "Counts");

    int photon_count = 0;
    int electron_count = 0;
    if (radiant_sizes.size() == cloud_sizes.size()) {
        for (int i = 0; i < radiant_sizes.size(); i++) {
            photon_count += radiant_sizes[i];
            electron_count += cloud_sizes[i];
        }
    }

    hist_light_fixed->Fill(photon_count);
    hist_charge_fixed->Fill(electron_count);
    hist_quanta_fixed->Fill(photon_count + electron_count);
}

void AnalysisManager::AnalyseFixedEnergyYield(const Scintillation* scintillation, const Ionisation* ionisation) {
    std::vector<double> visible_deposits = scintillation->get_visible_deposits();
    std::vector<double> linear_transfers = scintillation->get_linear_transfers();
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();

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
    
    hist_charge_yield = GetHistogram(TH1F_map_, "hist_charge_yield", "Charge", "Counts");
    hist_light_yield = GetHistogram(TH1F_map_, "hist_light_yield", "Light", "Counts");
    hist_quanta_yield = GetHistogram(TH1F_map_, "hist_quanta_yield", "Quanta", "Counts");

    hist_light_yield->Fill(photon_count / visible_deposit);
    hist_charge_yield->Fill(electron_count / visible_deposit);
    hist_quanta_yield->Fill((photon_count + electron_count) / visible_deposit);
}

void AnalysisManager::AnalyseVariableEnergyYield(const Scintillation* scintillation, const Ionisation* ionisation) {
    std::vector<double> visible_deposits = scintillation->get_visible_deposits();
    std::vector<double> linear_transfers = scintillation->get_linear_transfers();
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> cloud_sizes = ionisation->get_cloud_sizes();

    hist_var_charge_yield = GetHistogram(TH2F_map_, "hist_var_charge_yield", "Charge", "Counts");
    hist_var_light_yield = GetHistogram(TH2F_map_, "hist_var_light_yield", "Light", "Counts");
    hist_var_quanta_yield = GetHistogram(TH2F_map_, "hist_var_quanta_yield", "Quanta", "Counts");

    hist_dQdx = GetHistogram(TH2F_map_, "hist_dQdx", "dQdx", "Counts");
    hist_dLdx = GetHistogram(TH2F_map_, "hist_dLdx", "dLdx", "Counts");
    hist_dJdx = GetHistogram(TH2F_map_, "hist_dJdx", "dJdx", "Counts");

    if (radiant_sizes.size() == cloud_sizes.size()) {
        for (int i = 0; i < radiant_sizes.size(); i++) {
            hist_var_charge_yield->Fill(linear_transfers[i], cloud_sizes[i] / visible_deposits[i]);
            hist_var_light_yield->Fill(linear_transfers[i], radiant_sizes[i] / visible_deposits[i]);
            hist_var_quanta_yield->Fill(linear_transfers[i], (cloud_sizes[i] + radiant_sizes[i]) / visible_deposits[i]);

            hist_dQdx->Fill(linear_transfers[i], cloud_sizes[i] / (linear_transfers[i] / visible_deposits[i]));
            hist_dLdx->Fill(linear_transfers[i], radiant_sizes[i] / (linear_transfers[i] / visible_deposits[i]));
            hist_dJdx->Fill(linear_transfers[i], (cloud_sizes[i] + radiant_sizes[i]) / (linear_transfers[i] / visible_deposits[i]));
        }
    }
}

void AnalysisManager::AnalysePulse(const Scintillation* scintillation) {
    std::vector<double> radiant_sizes = scintillation->get_radiant_sizes();
    std::vector<double> emission_times = scintillation->get_emission_times();

    hist_pulse = GetHistogram(TH1F_map_, "hist_pulse", "Time", "Counts", 1000, 0, 1000);

    for (int i = 0; i < emission_times.size(); i++) {
        hist_pulse->Fill(emission_times[i]);
    }
}