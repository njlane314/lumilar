#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <map>
#include <algorithm>
#include <set>
#include "TFile.h"
#include "TTree.h"

int main(int argc, char* argv[]) {
    TFile* outputRootFile = TFile::Open("../data/photon_efficiencies.root", "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file photon_efficiencies.root" << std::endl;
        return 1;
    }

    outputRootFile->cd();

    TTree* photonEfficiencyTree = new TTree("photon_efficiencies_tree", "Photon Efficiencies");

    double rEffic, rEfficError, rEnergy;

    photonEfficiencyTree->Branch("Efficiency", &rEffic, "Efficiency/D");
    photonEfficiencyTree->Branch("EfficiencyError", &rEfficError, "Efficiency Error/D");
    photonEfficiencyTree->Branch("Energy", &rEnergy, "Energy/D");

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
    };

    int nuEnergy = 4;
    for (const std::string& rootFileName : rootFileNames) {
        TFile* rootFile = TFile::Open(rootFileName.c_str(), "READ");
        if (!rootFile) {
            std::cerr << "-- Error: Cannot open root file " << rootFileName << std::endl;
            return 1;
        }

        TTree* photonTree = (TTree*)rootFile->Get("arrival_photons_tree");
        if (!photonTree) {
            std::cerr << "-- Error: Cannot find the photonTree in root file" << std::endl;
            rootFile->Close();
            return 1;
        }

        TTree* eventTree = (TTree*)rootFile->Get("event_tree");
        if (!eventTree) {
            std::cerr << "-- Error: Cannot find the eventTree in root file" << std::endl;
            rootFile->Close();
            return 1;
        }

        Long64_t nEntries = photonTree->GetEntries();
        std::cout << "Number of entries in the root file: " << nEntries << std::endl;

        std::vector<int> *photonSensorCounts = nullptr;
        int totalOpticalPhotons = 0;

        photonTree->SetBranchAddress("sensor_count", &photonSensorCounts);
        eventTree->SetBranchAddress("total_optical_photons", &totalOpticalPhotons);

        std::vector<double> collectionEfficiencies;

        for (Long64_t entry = 1; entry < nEntries; ++entry) {
            photonTree->GetEntry(entry);
            eventTree->GetEntry(entry);

            int totalCollectedPhotons = 0;
            for (int opt_det = 0; opt_det < photonSensorCounts->size(); ++opt_det) {
                int nPhotons = photonSensorCounts->at(opt_det);
                totalCollectedPhotons += nPhotons;
            }

            double collectionEffic = static_cast<double>(totalCollectedPhotons) / totalOpticalPhotons;
            collectionEfficiencies.push_back(collectionEffic);
        }

        
        double meanEffic = 0;
        for (int i = 0; i < collectionEfficiencies.size(); ++i) {
            meanEffic += collectionEfficiencies[i];
        }
        meanEffic /= collectionEfficiencies.size();

        double efficError = 0;
        for (int i = 0; i < collectionEfficiencies.size(); ++i) {
            efficError += (collectionEfficiencies[i] - meanEffic) * (collectionEfficiencies[i] - meanEffic);
        }
        efficError /= (collectionEfficiencies.size() - 1);
        efficError = std::sqrt(efficError);

        double standardErrorMean = efficError / std::sqrt(collectionEfficiencies.size());

        rEffic = meanEffic;
        rEfficError = standardErrorMean;
        rEnergy = nuEnergy;
    
        photonEfficiencyTree->Fill();

        rootFile->Close();
        nuEnergy++;
    }

    outputRootFile->Write();
    outputRootFile->Close();

    return 0;
}