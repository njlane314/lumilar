//____________________________________________________________________________
/*!

\class   geometry::DetectorConstruction

\brief   This class constructs the detector geometry.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

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

#include "DetectorMessenger.hh"
#include "MediumProperties.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction(DetectorMessenger* detector_messenger);
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();
    void GetDetectorDimensions(double& width, double& height, double& depth);

private:
    DetectorMessenger* detector_messenger_;

    std::string detector_name_;
    std::string detector_shape_;
    double detector_width_;
    double detector_height_;
    double detector_depth_;
    double detector_step_;
    double detector_xenon_;
    double detector_nitrogen_;
};


#endif // DETECTOR_CONSTRUCTION_HH
