#include "PulseShape.hh"

AnalysisResults<TH1F> PulseShape::results_;

void PulseShape::processSensors(const OpticalSensorVector &optical_sensors) {
    int optical_sensor_idx = 0;
    for (const auto& a_optical_sensor : optical_sensors) {
        optical_sensor_idx++;
        std::vector<double> arrival_times = a_optical_sensor->getPhotonTimes();
        results_.createHistogram(std::to_string(optical_sensor_idx), "detectedPhotons", "Time of arrival of detected photons", "Photons/bin", 2000, 0, 2000);
        for (const auto& arrival_time : arrival_times) {
            results_.fillHistogram(std::to_string(optical_sensor_idx), "detectedPhotons", arrival_time);
        }
    }
}

void PulseShape::writeToFile(const std::string& filename) {
    std::cout << "Writing to file: " << filename << "\n";
    results_.writeToFile(filename);
    results_.clear();
}
