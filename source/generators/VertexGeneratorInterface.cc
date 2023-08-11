#include "VertexGeneratorInterface.hh"
//_________________________________________________________________________________________
bool VertexGeneratorInterface::HasNextVertex() const {
    return true;
}
//_________________________________________________________________________________________
void VertexGeneratorInterface::ShootVertex(G4ThreeVector& vertex) {
    DetectorConstruction* detector_construction = (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	detector_construction->GetDetectorDimensions(detector_width_, detector_height_, detector_depth_);
    
    double x_pos = CLHEP::RandFlat::shoot(-detector_width_/2, detector_width_/2);
    double y_pos = CLHEP::RandFlat::shoot(-detector_height_/2, detector_height_/2);
    double z_pos = CLHEP::RandFlat::shoot(-detector_depth_/2, detector_depth_/2);

    vertex.setX(x_pos);
    vertex.setY(y_pos);
    vertex.setZ(z_pos);
}