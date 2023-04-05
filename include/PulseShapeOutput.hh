#ifndef PULSE_SHAPE_OUTPUT_HH
#define PULSE_SHAPE_OUTPUT_HH

#include <string>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include "TVectorD.h"

#include "Scintillation.hh"

#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TFile.h"
#include "TProfile.h"

class PulseShapeOutput {
public:
    PulseShapeOutput();
    virtual ~PulseShapeOutput();

    static PulseShapeOutput* Instance();

    void RecordEntry(const Scintillation* scintillation);
    void SaveFile();

    int xmin_;
    int xmax_;
    double time_resolution_;
    int nbins_;
    std::vector<double> probability_lookup_table_;
    std::vector<double> temporal_correlations_;

private:
    void CreateFile();
    TH1F* CreatePulseShape(const Scintillation* scintillation, const std::string& name);
    void WritePulseShape(TH1F* pulse_shape);
    TProfile* CrossCorrelation(const TH1F* pulse_shape, const std::string& name);
    void WriteCrossCorrelation(TProfile* cross_correlation);
    //double TemporalEnergyCorrelation(const TProfile* profile, double xmin, double xmax);
    void WriteTemporalCorrelation(TProfile* temporal_correlation);
    TProfile* TemporalCorrelation(const TProfile* cross_correlation, const std::string& name);

    double ReferencePulse(double t);

    static PulseShapeOutput* instance_;

    TFile* file_ = nullptr;
    TTree* pulse_shape_tree_ = nullptr;
};

#endif // PULSE_SHAPE_OUTPUT_HH