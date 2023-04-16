#include "PulseShape.hh"

AnalysisResults<TH1F> PulseShape::results_;
AnalysisResults<TProfile> PulseShape::auto_correlation_;
AnalysisResults<TH2F> PulseShape::run_analysis_;

void::PulseShape::processSignal(const Signal* signal) {
    int hist_identifier = CLHEP::RandFlat::shootInt(1000);
    std::vector<double> arrival_times = signal->get_scintillation()->get_emission_times();

    int event_identifier = CLHEP::RandFlat::shootInt(10000);

    std::stringstream hist_name;
    hist_name << "evt" << std::setfill('0') << std::setw(3) << event_identifier << "_emission_shape";

    double xmin = 0;
    double xmax = 2000;
    double time_resolution = 1.;
    int nbins = round((double)(xmax - xmin) / (double)time_resolution);

    results_.createHistogram(hist_name.str(), "Time of emission [ns]", "Optical photons/bin", nbins, xmin, xmax);
    for (const auto& arrival_time : arrival_times) {
        results_.fillHistogram(hist_name.str(), arrival_time);
    }

    TH1F* pulse_shape = results_.getHistogram(hist_name.str());

    std::stringstream auto_name;
    auto_name << "evt" << std::setfill('0') << std::setw(3) << event_identifier << "_auto_correlation";

    auto_correlation_.createHistogram(auto_name.str(), "Time lag [ns]", "Auto-correlation", 2*nbins, -xmax, xmax);
    for (int lag = -xmax; lag < xmax; lag += time_resolution){
        double sum = 0.;
        for (int i = 1; i <= nbins; i++) {
            if (i*time_resolution + lag >= xmin && i*time_resolution + lag < xmax) {
                sum += pulse_shape->GetBinContent(i) * pulse_shape->GetBinContent(i + lag/time_resolution);
            }
        }
        auto_correlation_.fillHistogram(auto_name.str(), lag, sum);
    }

    TProfile* auto_corr = auto_correlation_.getHistogram(auto_name.str());
    double auto_thresh = 3e7;
    double place_holder = 0;
    double cluster_range = 20; 
    std::vector<double> peaks;
    for (int i = 1000; i < 2*nbins; i++) {
        double y = auto_corr->GetBinContent(i);
        if (y > auto_thresh) {
            if (y > place_holder){
                place_holder = y;
            }
            else if (y < place_holder){
                place_holder = 0;
                double peak_pos = i*time_resolution - xmax;
                bool clustered = false;
                for (auto& peak : peaks) {
                    //std::cout << "Peak: " << peak << " Peak pos: " << peak_pos << std::endl;
                    if (std::abs(peak_pos - peak) <= cluster_range) {
                        peak = (peak + peak_pos) / 2.0;
                        clustered = true;
                        break;
                    }
                }

                if (!clustered) {
                    peaks.push_back(peak_pos);
                }
            }
        }
    }

    int bin_number = pulse_shape->FindBin(0);
    double t0_intgral = pulse_shape->Integral(bin_number, bin_number+20);
    double t1_integral;

    std::stringstream ratio_name;
    std::stringstream decay_name;
    int run_identifier = 000;
    ratio_name << "run" << std::setfill('0') << std::setw(3) << run_identifier << "_singlet_ratio";
    decay_name << "run" << std::setfill('0') << std::setw(3) << run_identifier << "_decay_times";

    std::vector<double> decay_times = signal->get_delay_times();
    if (decay_times.size() > 0) {
        for (const auto& decay_time : decay_times) {
            int bin_number = pulse_shape->FindBin(decay_time);
            std::cout << "Bin number " << bin_number << std::endl;
            std::cout << "decay time " << decay_time << std::endl;
            t1_integral = pulse_shape->Integral(bin_number, bin_number+20);
        }

        TH2F* run_analysis = run_analysis_.getHistogram(ratio_name.str());
        TH2F* decay_times = run_analysis_.getHistogram(decay_name.str());
        if (run_analysis == nullptr) {
            run_analysis_.createHistogram(ratio_name.str(), "Neutrino energy [MeV]", "Singlet ratio", 100, 1, 0);
            run_analysis = run_analysis_.getHistogram(ratio_name.str());
        }
        if (decay_times == nullptr) {
            run_analysis_.createHistogram(decay_name.str(), "Neutrino energy [MeV]", "Singlet decay time [ns]", 100, 1, 0);
            decay_times = run_analysis_.getHistogram(decay_name.str());
        }
        std::cout << "T0 integral " << t0_intgral << std::endl;
        std::cout << "T1 integral " << t1_integral << std::endl;
        std::cout << "Ratio " << (double)t1_integral/(double)t0_intgral << std::endl;
        run_analysis->Fill(signal->get_primary_energy(), (double)t1_integral/(double)t0_intgral);
        //decay_times->Fill(signal->get_primary_energy(), decay_times[0]);
    }

    
}

/*void PulseShape::processSensors(const OpticalSensorVector &optical_sensors) {
    int optical_sensor_idx = 0;
    for (const auto& a_optical_sensor : optical_sensors) {
        optical_sensor_idx++;
        std::vector<double> arrival_times = a_optical_sensor->getPhotonTimes();
        results_.createHistogram(std::to_string(optical_sensor_idx), "detectedPhotons", "Time of arrival of detected photons", "Photons/bin", 2000, 0, 2000);
        for (const auto& arrival_time : arrival_times) {
            results_.fillHistogram(std::to_string(optical_sensor_idx), "detectedPhotons", arrival_time);
        }
    }
}*/

void PulseShape::writeToFile(const std::string& filename) {
    results_.writeToFile(filename);
    results_.clear();

    auto_correlation_.writeToFile(filename);
    auto_correlation_.clear();
}

void PulseShape::writeAndFinish(const std::string& filename) {
    run_analysis_.writeToFile(filename);
    run_analysis_.clear();
}
