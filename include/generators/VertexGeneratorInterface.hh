//____________________________________________________________________________
/*!

\class   generators::VertexGeneratorInterface

\brief   This class interfaces with the vertex generator.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 13, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

#ifndef VERTEX_GENERATOR_INTERFACE_HH
#define VERTEX_GENERATOR_INTERFACE_HH

#include <G4ThreeVector.hh>
#include <G4RunManager.hh>
#include <G4VUserDetectorConstruction.hh>

#include "VertexGeneratorInterface.hh"
#include "DetectorConstruction.hh"

struct VertexGeneratorInterface {
    VertexGeneratorInterface() = default;
    virtual ~VertexGeneratorInterface() = default;

    virtual bool HasNextVertex() const;
    virtual void ShootVertex(G4ThreeVector &  vertex_);

    double detector_width_;
    double detector_height_;
    double detector_depth_;
};

#endif // VERTEX_GENERATOR_INTERFACE_HH