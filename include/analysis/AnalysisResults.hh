//____________________________________________________________________________
/*!

\class   analysis::AnalysisResults

\brief   This class stores and saves histograms on the stack.

\author  Nicholas Lane <nicholas.lane \at postgrad.manchester.ac.uk>, University of Manchester

\created May 11, 2023

\cpright GNU Public License
*/
//____________________________________________________________________________

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
#include <THStack.h>
#include <TCanvas.h>
#include <TImage.h>
#include <TLegend.h>
#include <TColor.h>
#include <TStyle.h>

#include "AnalysisManager.hh"

template <typename HistType,
          typename std::enable_if<std::is_same<HistType, TH1F>::value || std::is_same<HistType, TH2F>::value || std::is_same<HistType, TProfile>::value, bool>::type = true>
class AnalysisResults {
public:
    AnalysisResults() {}
    ~AnalysisResults() {}

    template<typename... Args>
    void FillHistogram(const std::string& name, Args... args) { // not sure if this function works properly
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
    }

    void CreateHistogram(const std::string& name, const std::string& xTitle, const std::string& yTitle, int nBinsX = 100, double xMin = 1., double xMax = 0., int nBinsY = 100, double yMin = 1., double yMax = 0.) {
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

    HistType* GetHistogram(const std::string& name) {
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

    void WriteToFile(const std::string& filename) {
        std::cout << "-- Writing histograms to file \"" << filename << "\"" << std::endl;
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

    void SaveHistograms() {
        std::string output_filename = AnalysisManager::GetInstance()->GetAnalysisFilename();
        std::cout << "-- Writing histograms to file \"" << output_filename << "\"" << std::endl;

        TFile file(output_filename.c_str(), "UPDATE");
        if (!file.IsOpen()) {
            std::cerr << "-- Error opening file \"" << output_filename << "\"" << std::endl;
            return;
        }

        for (const auto& hist : histograms_) {
            std::cout << "-- Writing histogram \"" << hist->GetName() << "\"" << std::endl;
            hist->Write(hist->GetName(), TObject::kOverwrite);
        }

        ClearHistograms();
    }

    void AppendToFile(const std::string& filename) {
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

    void ClearHistograms() {
        histograms_.clear();
    }

    void StackHistograms(std::string hist_identifier, std::string x_axis_label, std::string y_axis_label) {
        std::string hist_stack_name = hist_identifier + "_hist_stack";
        THStack* hist_stack = new THStack(hist_stack_name.c_str(), hist_stack_name.c_str());

        TLegend* legend = new TLegend(0.2, 0.85, 0.33, 0.2);
        legend->SetTextSize(0.015);
        legend->SetEntrySeparation(0.6);

        std::vector<std::pair<TH1F*, double>> hist_excited_states;
        for (const auto& hist : histograms_) {
            std::string hist_name = hist->GetName();
            if (hist_name.find(hist_identifier) != std::string::npos) {
                if constexpr (std::is_same_v<HistType, TH1F>) {
                    std::string suffix = "_hist";
                    std::string prefix = "cascade_level_";
                    size_t suffix_pos = hist_name.rfind(suffix);
                    size_t prefix_pos = hist_name.rfind(prefix);
                    if (suffix_pos != std::string::npos && prefix_pos != std::string::npos) {
                        std::string excited_state_str = hist_name.substr(prefix_pos + prefix.size(), suffix_pos - prefix_pos - prefix.size());
                        double excited_state = std::stod(excited_state_str);
                        hist_excited_states.emplace_back(hist.get(), excited_state);
                    }
                }
            }
        }

        std::sort(hist_excited_states.begin(), hist_excited_states.end(), [](const auto& a, const auto& b) { return a.second < b.second; });

        int color_index = 0;
        int color_cycle = 0;

        std::vector<int> colors = {kBlue, kGreen, kRed, kOrange, kMagenta, kCyan, kYellow, kGray, kBlack, kPink, kAzure, kTeal, kViolet, kSpring, kOrange};

        for (const auto& hist_excited_state : hist_excited_states) {
            TH1F* hist = hist_excited_state.first;
            hist->SetLineColor(colors[color_index % colors.size()] + color_cycle);
            hist->SetFillColor(colors[color_index % colors.size()] + color_cycle);
            color_index++;
            hist_stack->Add(hist);
            std::string legend_label = "Excited level " + std::to_string(hist_excited_state.second) + " MeV";
            legend->AddEntry(hist, legend_label.c_str());

            if (color_index == colors.size()) {
                color_cycle -= 2;
                color_index = 0;
            }
        }

        TCanvas* canvas = new TCanvas(hist_stack_name.c_str(), hist_stack_name.c_str(), 1000, 600);
        canvas->SetLeftMargin(0.15);
        canvas->SetBottomMargin(0.15);
        canvas->SetGrid();

        hist_stack->SetTitle("");
        hist_stack->Draw("hist");
        hist_stack->GetXaxis()->SetTitle(x_axis_label.c_str());
        hist_stack->GetYaxis()->SetTitle(y_axis_label.c_str());
        hist_stack->GetXaxis()->SetTitleSize(0.04);
        hist_stack->GetYaxis()->SetTitleSize(0.04);
        hist_stack->GetXaxis()->SetLabelSize(0.03);
        hist_stack->GetYaxis()->SetLabelSize(0.03);
        hist_stack->GetXaxis()->SetTitleOffset(1.0);
        hist_stack->GetYaxis()->SetTitleOffset(0.8);
        hist_stack->SetMinimum(0);

        legend->Draw();

        std::string output_image_filename = hist_stack_name + ".png";
        canvas->SaveAs(output_image_filename.c_str());

        delete canvas;
        delete hist_stack;
        delete legend;
    }

private:
    std::vector<std::unique_ptr<HistType>> histograms_;
};

#endif // ANALYSIS_RESULTS_HH
