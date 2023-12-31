#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

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
#include "G4UIcmdWith3Vector.hh"

#include "DetectorConstruction.hh"
#include "MarleyGenerator.hh"
#include "BxDecay0Generator.hh"

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

    G4UIdirectory* bxdecay0_directory_;
    G4UIcmdWithAString* set_bxdecay0_source_cmd_;

    G4UIdirectory* fixed_position_directory_;
    G4UIcmdWith3Vector* set_fixed_position_cmd_;

    G4String generator_type_;
    G4GeneralParticleSource* general_generator_;
    
    MarleyGenerator* marley_generator_;
    G4String marley_source_;

    BxDecay0Generator::PrimaryGeneratorAction* bxdecay0_generator_;

    std::string output_filename_;

    G4ThreeVector fixed_position_;
    bool use_fixed_position_ = false;
};

#endif // PRIMARY_GENERATOR_ACTION_HH