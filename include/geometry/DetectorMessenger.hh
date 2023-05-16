//____________________________________________________________________________
/*!

\class   geometry::DetectorConstruction

\brief   This class constructs the detector geometry.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef DETECTOR_MESSENGER_HH
#define DETECTOR_MESSENGER_HH

#include <iomanip>
#include <iterator>

#include "G4VUserDetectorConstruction.hh"
#include "G4UImessenger.hh"
#include "G4UserLimits.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Tokenizer.hh"

class DetectorMessenger : public G4UImessenger {
public:
    DetectorMessenger();
    ~DetectorMessenger();

    void SetDetectorParameters(std::string& detector_name, std::string& detector_shape, double& detector_width, double& detector_height, double& detector_depth, double& detector_step, double& detector_xenon, double& detector_nitrogen);

private:
    G4UIdirectory* detector_directory_;
    G4UIcommand* detector_name_cmd_;
    G4UIcommand* detector_shape_cmd_;
    G4UIcommand* detector_width_cmd_;
    G4UIcommand* detector_height_cmd_;
    G4UIcommand* detector_depth_cmd_;
    G4UIcommand* detector_step_cmd_;
    G4UIcommand* detector_xenon_cmd_;
    G4UIcommand* detector_nitrogen_cmd_;

    void SetNewValue(G4UIcommand* cmd, G4String new_value);

    std::string detector_name_;
    std::string detector_shape_;
    double detector_width_;
    double detector_height_;
    double detector_depth_;
    double detector_step_;
    double detector_xenon_;
    double detector_nitrogen_;
};


#endif // DETECTOR_MESSENGER_HH