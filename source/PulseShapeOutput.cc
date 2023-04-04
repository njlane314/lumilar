#include "PulseShapeOutput.hh"

PulseShapeOutput* PulseShapeOutput::instance_ = nullptr;

PulseShapeOutput::PulseShapeOutput() {
    xmin_ = 0;
    xmax_ = 2000;
    time_resolution_ = 1.;
    nbins_ = round((double)(xmax_ - xmin_) / (double)time_resolution_);

    for (int i = 0; i <= nbins_; i++) {
        double time = (double)i * time_resolution_;
        probability_lookup_table_.push_back(ReferencePulse(time));
    }
}

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

    TH1F* pulse_shape = new TH1F(name.c_str(), "Pulse shape", nbins_, xmin_, xmax_);
    pulse_shape->GetXaxis()->SetTitle("Emission time [ns]");
    pulse_shape->GetYaxis()->SetTitle("Photons/bin");

    for (int i = 0; i < emission_times.size(); i++) {
        pulse_shape->Fill(emission_times[i]);
    }

    return pulse_shape;
}

TProfile* PulseShapeOutput::CrossCorrelation(const TH1F* pulse_shape, const std::string& name) {
    TProfile* cross_correlation = new TProfile(name.c_str(), "Cross-correlation", 4000, -2000., 2000.);
    cross_correlation->GetXaxis()->SetTitle("Lag [ns]");
    cross_correlation->GetYaxis()->SetTitle("Cross-correlation");

    for (int lag = -2000; lag < 2000; lag += time_resolution_) {
        double sum = 0.;
        for (int i = 1; i <= nbins_; i++) {
            if (i*time_resolution_ + lag >= 0 && i*time_resolution_ + lag < 2000) {
                sum += pulse_shape->GetBinContent(i) * probability_lookup_table_[i*time_resolution_ + lag];
            }
        }
        cross_correlation->Fill(lag, sum);
    }

    return cross_correlation;
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

void PulseShapeOutput::WriteCrossCorrelation(TProfile* cross_correlation) {
    if (!file_) {
        std::cerr << "-- Error: root file not created!" << std::endl;
        return;
    }
    if (!cross_correlation) {
        std::cerr << "-- Error: cross correlation pointer is null!" << std::endl;
        return;
    }

    cross_correlation->Write();
}

double PulseShapeOutput::TemporalEnergyCorrelation(const TProfile* profile) {
    double integral = 0.;
    int nbins = profile->GetNbinsX();
    double x;
    double binwidth;

    double max_bin_content = 0.;

    for (int i = 1; i <= nbins; i++) {
        binwidth = profile->GetBinWidth(i);
        x = profile->GetXaxis()->GetBinCenter(i);
        integral += profile->GetBinContent(i) * x * binwidth;
        if (profile->GetBinContent(i) > max_bin_content) { 
            max_bin_content = profile->GetBinContent(i);
        }
    }

    return (max_bin_content != 0.) ? integral / max_bin_content : 0.;
}

void PulseShapeOutput::RecordEntry(const Scintillation* scintillation) {
    static int pulse_shape_count = 0;
    std::string pulse_shape_name = "pulse_shape" + std::to_string(pulse_shape_count++);
    std::string cross_correlation_name = "cross_correlation" + std::to_string(pulse_shape_count++);

    TH1F* pulse_shape = CreatePulseShape(scintillation, pulse_shape_name);
    TProfile* cross_correlation = CrossCorrelation(pulse_shape, cross_correlation_name);
    double TEC = TemporalEnergyCorrelation(cross_correlation);

    if (!file_) {
        CreateFile();
    }

    WritePulseShape(pulse_shape);
    WriteCrossCorrelation(cross_correlation);

    delete pulse_shape;
    delete cross_correlation;
}

double PulseShapeOutput::ReferencePulse(double t) {
    double A_s = 0.14;
    double A_t = 0.86;
    double singlet_lifetime = 5. * ns;
    double triplet_lifetime = 2100. * ns;
    double pdf = A_s/singlet_lifetime * std::exp(-t/singlet_lifetime) + A_t/triplet_lifetime * std::exp(-t/triplet_lifetime);
    double normalization = pdf * time_resolution_;
    return normalization;
}