#ifndef PULSESHAPE_HH
#define PULSESHAPE_HH

#include "AnalysisResults.hh"
#include "OpticalSensor.hh"
#include <string>
#include <vector>

class PulseShape {
public:
    static AnalysisResults<TH1F> results_;

    static void processSensors(const OpticalSensorVector &optical_sensors);
    static void writeToFile(const std::string& filename);
};

#endif // PULSESHAPE_HH
