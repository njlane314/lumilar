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

void PulseShapeOutput::InitialiseTree() {
    file_ = new TFile("pulse_shapes.root", "RECREATE");

    pulse_shape_tree_ = new TTree("pulse_shape_tree", "Pulse shape tree");
    pulse_shape_tree_->Branch("emission_times", &emission_times_vec);
}

void PulseShapeOutput::CloseFile() {
    if (!file_) {
        return;
    }
    else {
        pulse_shape_tree_->Write();

        file_->Close();
        std::cout << "-- Output done" << std::endl;
        delete file_;
    }
}

void PulseShapeOutput::RecordEntry(const Scintillation* scintillation) {
    std::vector<double> emission_times = scintillation->get_emission_times();
    TVectorD emission_times_vec(emission_times.size(), emission_times.data());
    pulse_shape_tree_->Fill();
}