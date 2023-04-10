#ifndef CALORIMETRY_HH
#define CALORIMETRY_HH

#include <string>

#include "Signal.hh"
#include "AnalysisResults.hh"

class Calorimetry {
public:
    static void processSignal(const Signal* signal);
    static void writeToFile(const std::string& filename);

private:
    static AnalysisResults<TH1F> results_;
};

#endif // CALORIMETRY_HH