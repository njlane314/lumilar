#ifndef ANALYSIS_RESULTS_HH
#define ANALYSIS_RESULTS_HH

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <ostream>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

template <typename HistType,
          typename std::enable_if<std::is_same<HistType, TH1F>::value || std::is_same<HistType, TH2F>::value, bool>::type = true>
class AnalysisResults {
public:
    AnalysisResults() {}
    ~AnalysisResults() {}

    template<typename... Args>
    void fillHistogram(const std::string& signalID, const std::string& name, Args... args) {
        std::cout << "-- Filling histogram \"" << name << "\" for signal ID \"" << signalID << "\"" << std::endl;
        for (const auto& hist : histograms_[signalID]) {
            if (hist->GetName() == name) {
                if constexpr (sizeof...(args) == 1) {
                    hist->Fill(args...);
                }
                else if constexpr (sizeof...(args) == 2) {
                    HistType* hist2D = dynamic_cast<HistType*>(hist.get());
                    if (hist2D) {
                        hist2D->Fill(args...);
                    }
                    else {
                        std::cerr << "-- Histogram with name \"" << name << "\" is not a 2D histogram for signal ID \"" << signalID << "\"" << std::endl;
                    }
                }
                else {
                    static_assert(sizeof...(args) == 1 || sizeof...(args) == 2, "Incorrect number of arguments for fillHistogram");
                }
                return;
            }
        }
        std::cerr << "-- Histogram with name \"" << name << "\" not found for signal ID \"" << signalID << "\", creating it now" << std::endl;
        if constexpr (sizeof...(args) == 1) {
            createHistogram(signalID, name, "", "", 100, 1, 0);
            fillHistogram(signalID, name, args...);
        }
        else if constexpr (sizeof...(args) == 2) {
            createHistogram(signalID, name, "", "", 100, 1, 0, 100, 1, 0);
            fillHistogram(signalID, name, args...);
        }
        else {
            static_assert(sizeof...(args) == 1 || sizeof...(args) == 2, "Incorrect number of arguments for fillHistogram");
        }
    }

    void createHistogram(const std::string& signalID, const std::string& name, const std::string& xTitle, const std::string& yTitle, int nBinsX, double xMin, double xMax, int nBinsY = 0, double yMin = 0.0, double yMax = 0.0) {
        std::unique_ptr<HistType> hist;
        std::cout << "-- Creating histogram \"" << name << "\" for signal ID \"" << signalID << "\"" << std::endl;

        if constexpr (std::is_same_v<HistType, TH1F>) {
            hist = std::make_unique<HistType>(name.c_str(), name.c_str(), nBinsX, xMin, xMax);
        }
        else if constexpr (std::is_same_v<HistType, TH2F>) {
            hist = std::make_unique<HistType>(name.c_str(), name.c_str(), nBinsX, xMin, xMax, nBinsY, yMin, yMax);
        }
        else {
            static_assert(std::is_same_v<HistType, TH1F> || std::is_same_v<HistType, TH2F>, "Incorrect histogram type");
        }

        hist->GetXaxis()->SetTitle(xTitle.c_str());
        hist->GetYaxis()->SetTitle(yTitle.c_str());
        histograms_[signalID].push_back(std::move(hist));
        std::cout << "-- Histogram \"" << name << "\" created for signal ID \"" << signalID << "\"" << std::endl;
    }

    void writeToFile(const std::string& filename) {
        TFile file(filename.c_str(), "UPDATE");
        if (!file.IsOpen()) {
            std::cerr << "-- Error opening file \"" << filename << "\"" << std::endl;
            return;
        }
        for (auto& histGroup : histograms_) {
            std::string signalID = histGroup.first;
            for (const auto& hist : histGroup.second) {
                hist->Write(("evt" + signalID + "_" + hist->GetName()).c_str(), TObject::kOverwrite);
            }
        }
        file.Close();
    }

    void clear() {
        histograms_.clear();
    }

private:
    std::map<std::string, std::vector<std::unique_ptr<HistType>>> histograms_;
};

#endif // ANALYSIS_RESULTS_HH
