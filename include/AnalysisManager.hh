#ifndef ANALYSIS_MANAGER_HH
#define ANALYSIS_MANAGER_HH

#include <iostream>
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

  void DiscreteResponse(const G4Event* event, const Scintillation* scintillation, const Ionisation* ionisation);
  void EventResponse(const G4Event* event, const Scintillation* scintillation, const Ionisation* ionisation);
  void SignalYield(const Scintillation* scintillation, const Ionisation* ionisation);
  
  void StackPulseShape(const Scintillation* scintillation);
  void RandomPulseShape(const Scintillation* scintillation);
  void PulseShapeDiscrimination(const Scintillation* scintillation, const Ionisation* ionisation);

 private:
  static AnalysisManager* instance_;

  std::map<std::string, TH1F*> TH1F_map_;
  std::map<std::string, TH2F*> TH2F_map_;

  TH1F* charge_step_response;
  TH1F* light_step_response;
  TH1F* joint_step_response;
  TH1F* charge_event_response;
  TH1F* light_event_response;
  TH1F* joint_event_response;
  TH1F* charge_event_yield;
  TH1F* light_event_yield;
  TH1F* joint_event_yield;

  TH2F* charge_event_response_energy;
  TH2F* light_event_response_energy;
  TH2F* joint_event_response_energy;
  TH2F* charge_step_yield;
  TH2F* light_step_yield;
  TH2F* joint_step_yield;
  TH2F* charge_step_density;
  TH2F* light_step_density;
  TH2F* joint_step_density;
  TH2F* charge_event_visible_energy;
  TH2F* light_event_visible_energy;
  TH2F* joint_event_visible_energy;

  TH1F* pulse_shape;
  TH1F* random_pulse_shape;
  TH2F* prompt_fraction_scint;
  TH2F* prompt_fraction_ratio;
};

#endif // ANALYSIS_MANAGER_HH