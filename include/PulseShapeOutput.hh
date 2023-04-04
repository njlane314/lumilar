#ifndef PULSE_SHAPE_OUTPUT_HH
#define PULSE_SHAPE_OUTPUT_HH

#include <string>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include "TVectorD.h"

#include "Scintillation.hh"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

class PulseShapeOutput {
public:
    PulseShapeOutput();
    virtual ~PulseShapeOutput();

    static PulseShapeOutput* Instance();

    void RecordEntry(const Scintillation* scintillation);
    void SaveFile();

private:
    void CreateFile();
    TH1F* CreatePulseShape(const Scintillation* scintillation, const std::string& name);
    void WritePulseShape(TH1F* pulse_shape);

    static PulseShapeOutput* instance_;

    TFile* file_ = nullptr;
    TTree* pulse_shape_tree_ = nullptr;
};

#endif // PULSE_SHAPE_OUTPUT_HH