#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include "DetectorConstruction.hh"
#include "MarleyGenerator.hh"

#include <random>
#include <cmath>

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4UImessenger.hh"

#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4GeneralParticleSource.hh"
#include "MarleyGenerator.hh"
#include "G4UIcmdWithAString.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4SystemOfUnits.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction, public G4UImessenger {
public:
    PrimaryGeneratorAction(std::string output_filename);
    ~PrimaryGeneratorAction();

    void SetNewValue(G4UIcommand* cmd, G4String new_value);
    void GeneratePrimaries(G4Event* anEvent);

    inline std::string getOutputFilename() const { return output_filename_; }

private:
    double detector_width_;
    double detector_height_;
    double detector_depth_;

    G4Box* detector_solid_;

    G4UIdirectory* generator_directory_;
    G4UIcmdWithAString* set_generator_cmd_;

    G4UIdirectory* marley_directory_;
    G4UIcmdWithAString* set_marley_source_cmd_;

    G4String generator_type_;
    G4GeneralParticleSource* general_generator_;
    
    MarleyGenerator* marley_generator_;
    G4String marley_source_;

    std::string output_filename_;
};

#endif // PRIMARY_GENERATOR_ACTION_HH