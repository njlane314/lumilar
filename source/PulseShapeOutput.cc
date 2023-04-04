#include "PulseShapeOutput.hh"

PulseShapeOutput* PulseShapeOutput::instance_ = nullptr;

PulseShapeOutput::PulseShapeOutput() {}

PulseShapeOutput::~PulseShapeOutput() {}

PulseShapeOutput* PulseShapeOutput::Instance() {
    if (!instance_) {
        instance_ = new PulseShapeOutput();
    }
    
    return instance_;
}

void PulseShapeOutput::CreateFile() {
    std::cout << "-- Creating pulse shape file" << std::endl;
    file_ = new TFile("pulse_shapes.root", "RECREATE");
}

void PulseShapeOutput::SaveFile() {
    if (!file_) {
        return;
    }
    else {
        file_->Close();
        std::cout << "-- Pulse shape analysis done" << std::endl;
        delete file_;
    }
}

TH1F* PulseShapeOutput::CreatePulseShape(const Scintillation* scintillation, const std::string& name) {
    std::vector<double> emission_times = scintillation->get_emission_times();

    int xmin = 0;
    int xmax = 2000;
    double time_resolution = 1.;
    int nbins = round((double)(xmax - xmin) / (double)time_resolution);

    TH1F* pulse_shape = new TH1F(name.c_str(), "Pulse shape", nbins, xmin, xmax);
    pulse_shape->GetXaxis()->SetTitle("Emission time [ns]");
    pulse_shape->GetYaxis()->SetTitle("Photons/bin");

    for (int i = 0; i < emission_times.size(); i++) {
        pulse_shape->Fill(emission_times[i]);
    }

    return pulse_shape;
}

void PulseShapeOutput::WritePulseShape(TH1F* pulse_shape) {
    if (!file_) {
        std::cerr << "-- Error: root file not created!" << std::endl;
        return;
    }
    if (!pulse_shape) {
        std::cerr << "-- Error: pulse shape pointer is null!" << std::endl;
        return;
    }

    pulse_shape->Write();
}


void PulseShapeOutput::RecordEntry(const Scintillation* scintillation) {
    static int pulse_shape_count = 0;
    std::string pulse_shape_name = "pulse_shape" + std::to_string(pulse_shape_count++);

    TH1F* pulse_shape = CreatePulseShape(scintillation, pulse_shape_name);

    if (!file_) {
        CreateFile();
    }

    WritePulseShape(pulse_shape);
    delete pulse_shape;
}
