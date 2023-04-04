#ifndef PULSE_SHAPE_OUTPUT_HH
#define PULSE_SHAPE_OUTPUT_HH

#include <string>
#include <TFile.h>
#include <TTree.h>
#include "TVectorD.h"

#include "Scintillation.hh"

class PulseShapeOutput {
public:
    PulseShapeOutput();
    virtual ~PulseShapeOutput();

    static PulseShapeOutput* Instance();

    void RecordEntry(const Scintillation* scintillation);
    void CloseFile();

private:
    void InitialiseTree();

    static PulseShapeOutput* instance_;

    TFile* file_ = nullptr;
    TTree* pulse_shape_tree_ = nullptr;

    TVectorD emission_times_vec;
};

#endif // PULSE_SHAPE_OUTPUT_HH