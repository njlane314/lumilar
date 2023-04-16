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
#include <TProfile.h>
#include <TObject.h>

template <typename HistType,
          typename std::enable_if<std::is_same<HistType, TH1F>::value || std::is_same<HistType, TH2F>::value || std::is_same<HistType, TProfile>::value, bool>::type = true>
class AnalysisResults {
public:
    AnalysisResults() {}
    ~AnalysisResults() {}

    template<typename... Args>
    void fillHistogram(const std::string& name, Args... args) {
        for (const auto& hist : histograms_) {
            if (hist->GetName() == name) {
                if constexpr (std::is_same_v<HistType, TH1F>) {
                    hist->Fill(args...);
                }
                else if constexpr (std::is_same_v<HistType, TH2F>) {
                    HistType* hist2D = dynamic_cast<HistType*>(hist.get());
                    if (hist2D) {
                        hist2D->Fill(args...);
                    }
                    else {
                        throw std::runtime_error("-- Error filling histogram");
                    }
                }
                else if constexpr (std::is_same_v<HistType, TProfile>) {
                    HistType* histProfile = dynamic_cast<HistType*>(hist.get());
                    if (histProfile) {
                        double first_arg = std::get<0>(std::forward_as_tuple(args...));
                        double second_arg = std::get<1>(std::forward_as_tuple(args...));
                        histProfile->Fill(first_arg, second_arg);
                    }
                    else {
                        throw std::runtime_error("-- Error filling histogram");
                    }
                }
                else {
                    static_assert(std::is_same_v<HistType, TH1F> || std::is_same_v<HistType, TH2F> || std::is_same_v<HistType, TProfile>, "-- Incorrect histogram type");
                }

                return;
            }
        }
        
        /*if constexpr (sizeof...(args) == 1) {
            createHistogram(name, "", "", 100, 1, 0);
            fillHistogram(name, args...);
        }
        else if constexpr (sizeof...(args) == 2) {
            createHistogram(name, "", "", 100, 1, 0, 100, 1, 0);
            fillHistogram(name, args...);
        }
        else {
            static_assert(sizeof...(args) == 1 || sizeof...(args) == 2, "-- Incorrect number of arguments for fillHistogram");
        }*/
    }

    void createHistogram(const std::string& name, const std::string& xTitle, const std::string& yTitle, int nBinsX = 100, double xMin = 1., double xMax = 0., int nBinsY = 100, double yMin = 1., double yMax = 0.) {
        std::unique_ptr<HistType> hist;

        if constexpr (std::is_same_v<HistType, TH1F>) {
            hist = std::make_unique<HistType>(name.c_str(), name.c_str(), nBinsX, xMin, xMax);
        }
        else if constexpr (std::is_same_v<HistType, TH2F>) {
            hist = std::make_unique<HistType>(name.c_str(), name.c_str(), nBinsX, xMin, xMax, nBinsY, yMin, yMax);
        }
        else if constexpr (std::is_same_v<HistType, TProfile>) {
            hist = std::make_unique<HistType>(name.c_str(), name.c_str(), nBinsX, xMin, xMax);
        }
        else {
            static_assert(std::is_same_v<HistType, TH1F> || std::is_same_v<HistType, TH2F>, "-- Incorrect histogram type");
        }

        hist->GetXaxis()->SetTitle(xTitle.c_str());
        hist->GetYaxis()->SetTitle(yTitle.c_str());
        
        histograms_.push_back(std::move(hist));
    }

    HistType* getHistogram(const std::string& name) {
        for (const auto& hist : histograms_) {
            if (hist->GetName() == name) {
                if constexpr (std::is_same_v<HistType, TH1F>) {
                    return static_cast<HistType*>(hist.get());
                }
                else if constexpr (std::is_same_v<HistType, TH2F>) {
                    return dynamic_cast<HistType*>(hist.get());
                }
                else if constexpr (std::is_same_v<HistType, TProfile>) {
                    return dynamic_cast<HistType*>(hist.get());
                }
            }
        }

        return nullptr;
    }

    void writeToFile(const std::string& filename) {
        TFile file(filename.c_str(), "UPDATE");
        if (!file.IsOpen()) {
            std::cerr << "-- Error opening file \"" << filename << "\"" << std::endl;
            return;
        }

        for (const auto& hist : histograms_) {
            hist->Write(hist->GetName(), TObject::kOverwrite);
        }

        file.Close();
    }

    void appendToFile(const std::string& filename) {
        std::cout << "-- Appending histograms to file \"" << filename << "\"" << std::endl;
        TFile file(filename.c_str(), "UPDATE");
        if (!file.IsOpen()) {
            std::cerr << "-- Error opening file \"" << filename << "\"" << std::endl;
            return;
        }

        for (const auto& hist : histograms_) {
            TObject* obj = file.Get(hist->GetName());
            if (dynamic_cast<TH1*>(obj)) {
                hist->Add(dynamic_cast<TH1*>(obj));
            } else {
                hist->Write(hist->GetName(), TObject::kOverwrite);
            }
        }

        file.Close();
    }

    void clear() {
        histograms_.clear();
    }


private:
    std::vector<std::unique_ptr<HistType>> histograms_;
};

#endif // ANALYSIS_RESULTS_HH
