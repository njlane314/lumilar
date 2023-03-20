#ifndef OUTPUT_MANAGER_HH
#define OUTPUT_MANAGER_HH

#include "DetectorConstruction.hh"

#include <string>
#include <TFile.h>
#include <TTree.h>

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4UImessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include "Scintillation.hh"

class OutputManager :  public G4UImessenger{
public:
    OutputManager();
    virtual ~OutputManager();

    static OutputManager* Instance();

    void SetNewValue(G4UIcommand* cmd, G4String new_value);

    void RecordEntry(const G4Run* run);
    void RecordEntry(const G4Event* event);
    void RecordEntry(const G4Track* track);
    void RecordEntry(const G4Step* step);
    void RecordEntry(const Scintillation* scintillation);

    void CloseFile();

private:
    void CreateFile();

    static OutputManager* instance_;

    G4UIdirectory* output_directory_;
    G4UIcmdWithAString* set_filename_cmd_;

    std::string filename_;
    TFile* file_ = nullptr;
    TTree* meta_tree_ = nullptr;
    TTree* event_tree_ = nullptr;
    TTree* track_tree_ = nullptr;
    TTree* step_tree_ = nullptr;
    TTree* photon_tree_ = nullptr;

    G4Box* detector_solid_ = nullptr;
    
    Double_t detector_width;
    Double_t detector_height;
    Double_t detector_depth;
    
    Int_t parent_track_id;
    Int_t track_particle;
    Double_t track_position_x;
    Double_t track_position_y;
    Double_t track_position_z;
    Double_t track_momentum_x;
    Double_t track_momentum_y;
    Double_t track_momentum_z;
    Double_t track_time;
    Double_t track_total_energy;
    Double_t track_kinetic_energy;
    Double_t track_length;

    Int_t step_track_id;
    Double_t step_energy_deposit;
    Double_t step_non_ionizing_energy_deposit;
    Double_t step_position_x;
    Double_t step_position_y;
    Double_t step_position_z;
    Double_t step_momentum_x;
    Double_t step_momentum_y;
    Double_t step_momentum_z;
    Double_t step_time;
    Double_t step_total_energy;
    Double_t step_kinetic_energy;
    Double_t step_length;

    int step_count;
    int track_count;

    Double_t photon_time;
    Double_t photon_num;
};

#endif // OUTPUT_MANAGER_HH