#include <vector>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

bool isPrompt(double t, double promptWindow) {
    return t <= promptWindow;
}

double calculateFPrompt(const std::vector<std::vector<double>>* photonSensorTimes, double promptWindow) {
    std::vector<double> allPhotonTimes;
    for (const auto& times : *photonSensorTimes) {
        allPhotonTimes.insert(allPhotonTimes.end(), times.begin(), times.end());
    }

    int promptPhotonCount = std::count_if(allPhotonTimes.begin(), allPhotonTimes.end(), [promptWindow](double t) { return isPrompt(t, promptWindow); });
    return (allPhotonTimes.size() > 0) ? static_cast<double>(promptPhotonCount) / allPhotonTimes.size() : 0.0;
}

int main() {
    TFile* inputFile = TFile::Open("input_file_path.root", "READ");
    TTree* photonTree = (TTree*)inputFile->Get("arrival_photons_tree");

    std::vector<std::vector<double>>* photonSensorTimes = nullptr;
    photonTree->SetBranchAddress("sensor_arrival_times", &photonSensorTimes);

    TFile* outputFile = TFile::Open("fprompt_values.root", "RECREATE");
    TTree* outputTree = new TTree("fprompt_tree", "F-Prompt Values Tree");

    std::vector<double> promptWindows = {1.0, 2.0, 5.0, 10.0, 20.0, 50.0};  // Adjust this list based on your desired prompt thresholds
    std::vector<double> fPromptValues(promptWindows.size(), 0.0);
    for (size_t i = 0; i < promptWindows.size(); ++i) {
        outputTree->Branch(("fPrompt_" + std::to_string(promptWindows[i])).c_str(), &fPromptValues[i], ("fPrompt_" + std::to_string(promptWindows[i]) + "/D").c_str());
    }

    Long64_t nEntries = photonTree->GetEntries();
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
        photonTree->GetEntry(entry);
        for (size_t i = 0; i < promptWindows.size(); ++i) {
            fPromptValues[i] = calculateFPrompt(photonSensorTimes, promptWindows[i]);
        }
        outputTree->Fill();
    }

    outputFile->Write();
    outputFile->Close();
    inputFile->Close();
}
