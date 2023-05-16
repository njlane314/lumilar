#ifndef BULK_VERTEX_INTERFACE_HH
#define BULK_VERTEX_INTERFACE_HH

#include <G4ThreeVector.hh>
#include <G4RunManager.hh>
#include <G4VUserDetectorConstruction.hh>

#include "VertexGeneratorInterface.hh"
#include "DetectorConstruction.hh"

class BulkVertexGenerator : public VertexGeneratorInterface {
public: 
    BulkVertexGenerator();
    ~BulkVertexGenerator() override;

    void SetBulkVolume();
    void ShootVertex(G4ThreeVector & vertex_) override;

    double detector_width_;
    double detector_height_;
    double detector_depth_;
};

#endif // BULK_VERTEX_INTERFACE_HH