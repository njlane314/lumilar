#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4UImessenger.hh"
#include "G4UserLimits.hh"

#include <iomanip>
#include <iterator>

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"

#include "G4Material.hh"

#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4UniformElectricField.hh"
#include "G4FieldManager.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    struct Detector {
        std::string name;
        std::string material;
        std::string shape;
        double width;
        double height;
        double depth;
        double position[3];
        double step;
    };

    Detector detector_;

    DetectorConstruction(std::string detector_config);
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();
    void GetDetectorDimensions(double& width, double& height, double& depth);

private:
    std::string detector_config_;

    void ConstructAnodePlane();
    Detector ReadDetector(std::stringstream& ss);
};


#endif // DETECTOR_CONSTRUCTION_HH
