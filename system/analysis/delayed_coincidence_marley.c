#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <map>
#include <algorithm>
#include <cmath> 
#include <set>
#include "TFile.h"
#include "TTree.h"

int main(int argc, char* argv[]) {
    std::string placementFileName = "/home/lane/Software/src/lumilar/system/detector/placements/dune_solar_placements.txt";

    TFile* outputRootFile = TFile::Open("../data/delayed_coincidence_marley.root", "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file delayed_coincidence_marley.root" << std::endl;
        return 1;
    }

    outputRootFile->cd();

    TTree* delayedCoincidenceTree = new TTree("delayed_coincidence_marley_tree", "Delayed Coincidence Marley Tree");

    double rFracDelayed, rFracDelayedError, rEnergy;
    double rFracNeutron, rFracNeutronError;
    double rFracProton, rFracProtonError;
    double rFracGamma, rFracGammaError;

    delayedCoincidenceTree->Branch("FracDelayed", &rFracDelayed, "FracDelayed/D");
    delayedCoincidenceTree->Branch("FracDelayedError", &rFracDelayedError, "FracDelayedError/D");
    delayedCoincidenceTree->Branch("Energy", &rEnergy, "Energy/D");

    delayedCoincidenceTree->Branch("FracNeutron", &rFracNeutron, "FracNeutron/D");
    delayedCoincidenceTree->Branch("FracNeutronError", &rFracNeutronError, "FracNeutronError/D");

    delayedCoincidenceTree->Branch("FracProton", &rFracProton, "FracProton/D");
    delayedCoincidenceTree->Branch("FracProtonError", &rFracProtonError, "FracProtonError/D");

    delayedCoincidenceTree->Branch("FracGamma", &rFracGamma, "FracGamma/D");
    delayedCoincidenceTree->Branch("FracGammaError", &rFracGammaError, "FracGammaError/D");

    std::string BIN_PATH = "/home/lane/Software/src/lumilar/system/data/";
    std::vector<std::string> rootFileNames = {
        BIN_PATH + "marley_mono_5.root",
        BIN_PATH + "marley_mono_6.root",
        BIN_PATH + "marley_mono_7.root",
        BIN_PATH + "marley_mono_8.root",
        BIN_PATH + "marley_mono_9.root",
        BIN_PATH + "marley_mono_10.root",
        BIN_PATH + "marley_mono_11.root",
        BIN_PATH + "marley_mono_12.root",
        BIN_PATH + "marley_mono_13.root",
        BIN_PATH + "marley_mono_14.root",
        BIN_PATH + "marley_mono_15.root",
        BIN_PATH + "marley_mono_16.root",
        BIN_PATH + "marley_mono_17.root",
        BIN_PATH + "marley_mono_18.root",
        BIN_PATH + "marley_mono_19.root",
        BIN_PATH + "marley_mono_20.root",
        BIN_PATH + "marley_mono_21.root",
        BIN_PATH + "marley_mono_22.root",
        BIN_PATH + "marley_mono_23.root",
        BIN_PATH + "marley_mono_24.root",
        BIN_PATH + "marley_mono_25.root",
        BIN_PATH + "marley_mono_26.root",
        BIN_PATH + "marley_mono_27.root",
    };

    int nuEnergy = 4;
    for (const std::string& rootFileName : rootFileNames) {
        TFile* rootFile = TFile::Open(rootFileName.c_str(), "READ");
        if (!rootFile) {
            std::cerr << "-- Error: Cannot open root file " << rootFileName << std::endl;
            return 1;
        }

        TTree* marleyTree = (TTree*)rootFile->Get("marley_generator_tree");
        if (!marleyTree) {
            std::cerr << "-- Error: Cannot find the marleyTree in root file" << std::endl;
            rootFile->Close();
            return 1;
        }

        Long64_t nEntries = marleyTree->GetEntries();
        std::cout << "Number of entries in the root file: " << nEntries << std::endl;

        std::vector<double>* cascadeLevels = nullptr;
        int decayType = -1;

        marleyTree->SetBranchAddress("marley_generator_cascade_levels", &cascadeLevels);
        marleyTree->SetBranchAddress("marley_generator_decay_type", &decayType);

        int nDelayed = 0;
        int nNeutron = 0;
        int nProton = 0;
        int nGamma = 0;

        for (Long64_t entry = 1; entry < nEntries; ++entry) {
            marleyTree->GetEntry(entry);
            std::cout << "Processing entry " << entry << std::endl;
            
            for (double cascadeLevel : *cascadeLevels) {
                if (cascadeLevel == 1.64364) {
                    nDelayed++;
                }
            }

            if (decayType == 1) {
                nNeutron++;
            } else if (decayType == 2) {
                nProton++;
            } else {
                nGamma++;
            }
        }

        rFracDelayed = static_cast<double>(nDelayed) / nEntries;
        rFracDelayedError = sqrt(rFracDelayed * (1.0 - rFracDelayed) / nEntries);
        rEnergy = nuEnergy;

        rFracNeutron = static_cast<double>(nNeutron) / nEntries;
        rFracNeutronError = sqrt(rFracNeutron * (1.0 - rFracNeutron) / nEntries);

        rFracProton = static_cast<double>(nProton) / nEntries;
        rFracProtonError = sqrt(rFracProton * (1.0 - rFracProton) / nEntries);

        rFracGamma = static_cast<double>(nGamma) / nEntries;
        rFracGammaError = sqrt(rFracGamma * (1.0 - rFracGamma) / nEntries);

        delayedCoincidenceTree->Fill();
        rootFile->Close();

        nuEnergy++;
    }

    outputRootFile->Write();
    outputRootFile->Close();

    return 0;
}