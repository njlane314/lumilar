#ifndef ANALYSIS_MANAGER_HH
#define ANALYSIS_MANAGER_HH

#include <string>
#include <map>
#include <vector>

#include "G4Event.hh"
#include "Scintillation.hh"
#include "Ionisation.hh"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

class AnalysisManager {
 public:
    AnalysisManager();
    ~AnalysisManager();
    static AnalysisManager* Instance();
    void SaveHistograms();
    TH1F* CreateHistogram(const std::string& name, const std::string& x_axis_name, const std::string& y_axis_name, const int nbinsx, const double xmin, const double xmax);
    TH2F* CreateHistogram(const std::string& name, const std::string& x_axis_name, const std::string& y_axis_name, const int nbinsx, const double xmin, const double xmax, const int nbinsy, const double ymin, const double ymax);
    TH2F* GetHistogram(std::map<std::string, TH2F*>& hist_map, const std::string& name, const std::string& x_axis_name = "X Axis", const std::string& y_axis_name = "Y Axis", const int nbinsx = 100, const double xmin = 1, const double xmax = 0, const int nbinsy = 100, const double ymin = 1, const double ymax = 0);
    TH1F* GetHistogram(std::map<std::string, TH1F*>& hist_map, const std::string& name, const std::string& x_axis_name = "X Axis", const std::string& y_axis_name = "Y Axis", const int nbinsx = 100, const double xmin = 1, const double xmax = 0);
    void AnalyseFixedEnergySignal(const G4Event* event, const Scintillation* scintillation, const Ionisation* ionisation);
    void AnalyseVariableEnergySignal(const G4Event* event, const Scintillation* scintillation, const Ionisation* ionisation);
    void AnalyseSignalResponse(const G4Event* event, const Scintillation* scintillation, const Ionisation* ionisation);
    void AnalyseFixedEnergyYield(const Scintillation* scintillation, const Ionisation* ionisation);
 private:
    static AnalysisManager* instance_;
    std::map<std::string, TH1F*> TH1F_map_;
    std::map<std::string, TH2F*> TH2F_map_;
    TH1F* hist_charge;
    TH1F* hist_light;
    TH1F* hist_quanta;
    TH2F* hist_charge_energy;
    TH2F* hist_light_energy;
    TH2F* hist_quanta_energy;
    TH1F* hist_charge_fixed;
    TH1F* hist_light_fixed;
    TH1F* hist_quanta_fixed;
    TH1F* hist_charge_yield;
    TH1F* hist_light_yield;
    TH1F* hist_quanta_yield;
};

#endif // ANALYSIS_MANAGER_HH