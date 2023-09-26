#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <map>
#include <algorithm>
#include <filesystem>
#include <cmath> 
#include <set>
#include <numeric>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_filename>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];

    std::filesystem::path p(inputFilePath);
    std::string outputFileName = "../arrival_times_" + p.filename().string();
    std::cout << "Output file name: " << outputFileName << std::endl;
    TFile* outputRootFile = TFile::Open(outputFileName.c_str(), "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file!" << std::endl;
        return 1;
    }

    auto processFile = [&](const std::string& filename) {
        std::cout << "Processing file " << filename << std::endl;

        TFile* rootFile = TFile::Open(filename.c_str(), "READ");
        if (!rootFile) {
            std::cerr << "-- Error: Cannot open root file " << filename << std::endl;
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

        outputRootFile->cd();

        for (Long64_t entry = 1; entry < nEntries; ++entry) {
            photonTree->GetEntry(entry);
            
            std::cout << "Processing entry " << entry << std::endl;
            std::string histName = "arrivalTimeHist_" + std::to_string(entry);
            TH1F* hist = new TH1F(histName.c_str(), "Photon Arrival Times;Time;Counts", 2000, 0, 2000);

            for (const std::vector<double>& sensorArrivalTimes : *arrivalTimes) {
                for (const double time : sensorArrivalTimes) {
                    hist->Fill(time);
                }
            }
            
            std::cout << "Creating histogram: " << histName << std::endl;
            std::cout << "Saving histograms to: " << outputFileName << std::endl;

            hist->Write();
            delete hist;
        }

        rootFile->Close();

        return 0;
    };

    int ret = processFile(inputFilePath);
    if(ret != 0) {
        std::cerr << "-- Error: Problem processing the file." << std::endl;
        outputRootFile->Close();
        return 1;
    }

    outputRootFile->Write();
    outputRootFile->Close();

    return 0;
}