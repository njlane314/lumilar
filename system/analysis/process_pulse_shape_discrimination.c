#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <map>
#include <algorithm>
#include <set>
#include "TFile.h"
#include "TTree.h"

struct Event {
    double fPrompt;
    double energy; 
    bool isNeutrino;
};

std::pair<double, double> calculateEfficiencyAndRejection(const std::vector<Event>& events, double threshold) {
    int signalCount = 0;
    int signalAccepted = 0;
    int backgroundRejected = 0;

    for (const Event& event : events) {
        if (event.isNeutrino) {
            signalCount++;
            if (event.fPrompt >= threshold) {
                signalAccepted++;
            }
        } else if (event.fPrompt < threshold) {
            backgroundRejected++;
        }
    }

    double signalEfficiency = static_cast<double>(signalAccepted) / signalCount;
    double backgroundRejection = static_cast<double>(backgroundRejected) / (events.size() - signalCount);

    return { signalEfficiency, backgroundRejection };
}

double optimizeFPromptForBin(const std::vector<Event>& binEvents) {
    double optimalThreshold = 0.0;
    double maxSeparation = 0.0;

    for (double threshold = 0.0; threshold <= 1000.0; threshold++) {
        auto [signalEfficiency, backgroundRejection] = calculateEfficiencyAndRejection(binEvents, threshold);

        double separation = signalEfficiency + backgroundRejection;

        if (separation > maxSeparation) {
            maxSeparation = separation;
            optimalThreshold = threshold;
        }
    }

    return optimalThreshold;
}

double calculateFPrompt(const std::vector<std::vector<double>> photonTimes, const double promptTime) {
    int nPromptPhotons = 0;
    int nTotalPhotons = 0;

    for (const std::vector<double>& photonTimesForSensor : photonTimes) {
        for (const double& photonTime : photonTimesForSensor) {
            nTotalPhotons++;
            if (photonTime <= promptTime) {
                nPromptPhotons++;
            }
        }
    }

    return static_cast<double>(nPromptPhotons) / nTotalPhotons;
}

int main(int argc, char* argv[]) {
    TFile* outputRootFile = TFile::Open("../data/prompt_fraction.root", "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file prompt_fraction.root" << std::endl;
        return 1;
    }

    outputRootFile->cd();

    TTree* promptFractionTree = new TTree("prompt_fraction_tree", "Prompt Fraction Tree");

    std::string BIN_PATH = "/home/lane/Software/src/lumilar/system/data/";

    std::string signalFilename = BIN_PATH + "marley_mono_5.root";
    std::string backgroundFilename = BIN_PATH + "marley_mono_5.root";
       
    TFile* signalRootFile = TFile::Open(signalFilename.c_str(), "READ");
    if (!signalRootFile) {
        std::cerr << "-- Error: Cannot open root file " << signalFileName << std::endl;
        return 1;
    }

    TFile* backgroundRootFile = TFile::Open(backgroundFilename.c_str(), "READ");
    if (!backgroundRootFile) {
        std::cerr << "-- Error: Cannot open root file " << backgroundFileName << std::endl;
        return 1;
    }

    TTree* signalPhotonTree = (TTree*)signalRootFile->Get("arrival_photons_tree");
    if (!signalPhotonTree) {
        std::cerr << "-- Error: Cannot find the photonTree in signal root file" << std::endl;
        rootFile->Close();
        return 1;
    }

    TTree* backgroundPhotonTree = (TTree*)backgroundRootFile->Get("arrival_photons_tree");
    if (!backgroundPhotonTree) {
        std::cerr << "-- Error: Cannot find the photonTree in background root file" << std::endl;
        rootFile->Close();
        return 1;
    }

    std::vector<std::vector<double>>* signalPhotonTimes = nullptr;
    std::vector<std::vector<double>>* backgroundPhotonTimes = nullptr;

    signalPhotonTree->SetBranchAddress("sensor_arrival_times", &signalPhotonTimes);
    backgroundPhotonTree->SetBranchAddress("sensor_arrival_times", &backgroundPhotonTimes);

    std::vector<Event> signalEvents;

    Long64_t nSignalEntries = signalPhotonTree->GetEntries();

    for (Long64_t i = 0; i < nSignalEntries; i++) {
        signalPhotonTree->GetEntry(i);

        

        Long64_t nEntries = photonTree->GetEntries();
        std::cout << "Number of entries in the root file: " << nEntries << std::endl;

        std::vector<std::vector<double>>* photonTimes = nullptr;

        photonTree->SetBranchAddress("sensor_arrival_times", &photonTimes);

        for (Long64_t i = 0; i < nEntries; i++) {
            // create events
            photonTree->GetEntry(i);

            double fPrompt = calculateFPrompt(*photonTimes, 0.0);

    
        }

        rootFile->Close();

        nuEnergy++;
    }

    outputRootFile->Write();
    outputRootFile->Close();

    return 0;
}