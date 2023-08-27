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
    TFile* outputRootFile = TFile::Open("../data/decision_tree_data.root", "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file!" << std::endl;
        return 1;
    }

    outputRootFile->cd();

    TTree* decisionTreeDataTree = new TTree("decision_tree_data_tree", "Decision Tree Data Tree");

    int rSensorCount;
    double rFprompt90;
    double rFprompt200;
    int isSignal;

    decisionTreeDataTree->Branch("rSensorCount", &rSensorCount, "rSensorCount/I");
    decisionTreeDataTree->Branch("rFprompt90", &rFprompt90, "rFprompt90/D");
    decisionTreeDataTree->Branch("rFprompt200", &rFprompt200, "rFprompt200/D");
    decisionTreeDataTree->Branch("isSignal", &isSignal, "isSignal/I");

    std::string BIN_PATH = "/home/lane/Software/src/lumilar/system/data/";
    std::vector<std::string> signalFilenames = {
        BIN_PATH + "marley_B8_CC.root", 
        BIN_PATH + "marley_HEP_CC.root",
    };

    std::vector<std::string> backgroundFilenames = {
        BIN_PATH + "bxdecay0_Ar39.root", 
        BIN_PATH + "bxdecay0_Ar42.root",
        BIN_PATH + "bxdecay0_Bi214.root",
        BIN_PATH + "bxdecay0_K42.root",
        BIN_PATH + "bxdecay0_Kr85.root",
        BIN_PATH + "bxdecay0_Pb214.root",
        BIN_PATH + "bxdecay0_Rn222.root",
    };

    for (const std::string& signalFilename : signalFilenames) {
        std::cout << "Processing signal file " << signalFilename << std::endl;
        isSignal = 1;
        TFile* rootFile = TFile::Open(signalFilename.c_str(), "READ");
        if (!rootFile) {
            std::cerr << "-- Error: Cannot open root file " << signalFilename << std::endl;
            return 1;
        }

        TTree* photonTree = (TTree*)rootFile->Get("arrival_photons_tree");
        if (!photonTree) {
            std::cerr << "-- Error: Cannot find the photonTree in root file" << std::endl;
            rootFile->Close();
            return 1;
        }

        std::vector<int> *sensorCounts = nullptr;
        std::vector<std::vector<double>> *arrivalTimes = nullptr;
        photonTree->SetBranchAddress("sensor_count", &sensorCounts);
        photonTree->SetBranchAddress("sensor_arrival_times", &arrivalTimes);

        Long64_t nEntries = photonTree->GetEntries();
        std::cout << "Number of entries in the root file: " << nEntries << std::endl;

        for (Long64_t entry = 1; entry < nEntries; ++entry) {
            photonTree->GetEntry(entry);
            std::cout << "Processing entry " << entry << std::endl;

            int count90 = 0;
            int count200 = 0;
            int count10000 = 0;

            for (const std::vector<double>& sensorArrivalTimes : *arrivalTimes) {
                for (const double time : sensorArrivalTimes) {
                    if (time < 90.0 && time > 1) count90++;   
                    if (time < 200.0 && time > 1) count200++; 
                    if (time < 10000.0 && time > 1) count10000++;
                }
            }

            rFprompt90 = static_cast<double>(count90) / count10000;
            rFprompt200 = static_cast<double>(count200) / count10000;

            decisionTreeDataTree->Fill();
        }
    }

    for (const std::string& backgroundFilename : backgroundFilenames) {
        std::cout << "Processing background file " << backgroundFilename << std::endl;
        isSignal = 0;  
        TFile* rootFile = TFile::Open(backgroundFilename.c_str(), "READ");
        if (!rootFile) {
            std::cerr << "-- Error: Cannot open root file " << backgroundFilename << std::endl;
            return 1;
        }

        TTree* photonTree = (TTree*)rootFile->Get("arrival_photons_tree");
        if (!photonTree) {
            std::cerr << "-- Error: Cannot find the photonTree in root file" << std::endl;
            rootFile->Close();
            return 1;
        }

        std::vector<int> *sensorCounts = nullptr;
        std::vector<std::vector<double>> *arrivalTimes = nullptr;
        photonTree->SetBranchAddress("sensor_count", &sensorCounts);
        photonTree->SetBranchAddress("sensor_arrival_times", &arrivalTimes);

        Long64_t nEntries = photonTree->GetEntries();
        std::cout << "Number of entries in the root file: " << nEntries << std::endl;

        for (Long64_t entry = 1; entry < nEntries; ++entry) {
            photonTree->GetEntry(entry);
            std::cout << "Processing entry " << entry << std::endl;

            int count90 = 0;
            int count200 = 0;
            int count10000 = 0;

            for (const std::vector<double>& sensorArrivalTimes : *arrivalTimes) {
                for (const double time : sensorArrivalTimes) {
                    if (time < 90.0 && time > 1) count90++;   
                    if (time < 200.0 && time > 1) count200++; 
                    if (time < 10000.0 && time > 1) count10000++;
                }
            }

            rFprompt90 = static_cast<double>(count90) / count10000;
            rFprompt200 = static_cast<double>(count200) / count10000;

            decisionTreeDataTree->Fill();
        }

        rootFile->Close();
    }

    outputRootFile->Write();
    outputRootFile->Close();

    return 0;
}