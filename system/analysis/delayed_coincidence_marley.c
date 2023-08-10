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

    delayedCoincidenceTree->Branch("FracDelayed", &rFracDelayed, "FracDelayed/D");
    delayedCoincidenceTree->Branch("FracDelayedError", &rFracDelayedError, "FracDelayedError/D");
    delayedCoincidenceTree->Branch("Energy", &rEnergy, "Energy/D");

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

        marleyTree->SetBranchAddress("marley_generator_cascade_levels", &cascadeLevels);

        int nDelayed = 0;
        for (Long64_t entry = 1; entry < nEntries; ++entry) {
            marleyTree->GetEntry(entry);
            std::cout << "Processing entry " << entry << std::endl;
            
            for (double cascadeLevel : *cascadeLevels) {
                if (cascadeLevel == 1.64364) {
                    nDelayed++;
                }
            }
        }

        rFracDelayed = static_cast<double>(nDelayed) / nEntries;
        rFracDelayedError = sqrt(rFracDelayed * (1.0 - rFracDelayed) / nEntries);
        rEnergy = nuEnergy;

        delayedCoincidenceTree->Fill();
        rootFile->Close();

        nuEnergy++;
    }

    outputRootFile->Write();
    outputRootFile->Close();

    return 0;
}