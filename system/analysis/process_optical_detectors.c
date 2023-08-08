#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <map>
#include <algorithm>
#include <set>
#include "TFile.h"
#include "TTree.h"

std::vector<std::tuple<double, double, double>> readPlacementFile(const std::string& placementFileName) {
    std::ifstream placementFile(placementFileName);
    std::vector<std::tuple<double, double, double>> sensorPositions;

    if (!placementFile.is_open()) {
        std::cerr << "-- Error: Unable to open placement file " << placementFileName << std::endl;
        return sensorPositions;
    }

    double detector_width, detector_height, detector_depth;
    placementFile >> detector_width >> detector_height >> detector_depth;

    std::string sensor_shape, plane_orientation;
    double x, y, z, width, height;

    while (placementFile >> sensor_shape >> plane_orientation >> x >> y >> z >> width >> height) {
        sensorPositions.push_back(std::make_tuple(x, y, z));
    }

    placementFile.close();
    return sensorPositions;
}

bool isTriggered(const std::vector<std::tuple<double, double, double>>& sensorPositions,
                 const std::vector<std::vector<double>>* photonArrivalTimes,
                 int M, int N) {
    double QUANTUM_EFFIC = 0.1;
    double LIVE_SPACE = 0.1;                

    std::vector<int> photonsCollected(sensorPositions.size(), 0);

    for (int opt_det = 0; opt_det < sensorPositions.size(); ++opt_det) {
        if (opt_det >= photonArrivalTimes->size()) {
            std::cout << "-- Error: sensor position index out of range." << std::endl;
            return false; 
        }

        int nPhotons = photonArrivalTimes->at(opt_det).size();
        int photonCount = 0;
        for (int photon = 0; photon < nPhotons; ++photon) {
            if (photonArrivalTimes->at(opt_det)[photon] > 0.0) {
                photonCount++;
            }
        }
        photonsCollected[opt_det] = static_cast<int>(photonCount * QUANTUM_EFFIC * LIVE_SPACE);
    }

    int triggeredSensorCount = 0;
    for (int i = 0; i < photonsCollected.size(); ++i) {
        if (photonsCollected[i] >= N) {
            triggeredSensorCount++;
        }
    }

    return triggeredSensorCount >= M;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <root_file_name> <placement_file_name>" << std::endl;
        return 1;
    }

    std::string rootFileName = argv[1];
    std::string placementFileName = argv[2];

    TFile* rootFile = TFile::Open(rootFileName.c_str(), "READ");
    if (!rootFile) {
        std::cerr << "-- Error: Cannot open root file " << rootFileName << std::endl;
        return 1;
    }
        
	std::vector<std::vector<double>> *photonArrivalTimes = nullptr;
    std::vector<std::tuple<double, double, double>> sensorPositions = readPlacementFile(placementFileName);
        
    TTree* photonTree = (TTree*)rootFile->Get("arrival_photons_tree");
    if (!photonTree) {
        std::cerr << "-- Error: Cannot find the photonTree in root file" << std::endl;
        rootFile->Close();
        return 1;
    }

    Long64_t nEntries = photonTree->GetEntries();
    std::cout << "Number of entries in the root file: " << nEntries << std::endl;

    photonTree->SetBranchAddress("sensor_arrival_times", &photonArrivalTimes);

    std::string outputRootFileName = "trigger_efficiencies.root";
    TFile* outputRootFile = TFile::Open(outputRootFileName.c_str(), "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file " << outputRootFileName << std::endl;
        return 1;
    }

    TTree* efficiencyTree = new TTree("trigger_efficiencies_tree", "Trigger Efficiencies");
    int rootM, rootN;
    int rootSensorPhotons;
    double rootEfficiency;
    efficiencyTree->Branch("M", &rootM, "M/I");
    efficiencyTree->Branch("N", &rootN, "N/I");
    efficiencyTree->Branch("Efficiency", &rootEfficiency, "Efficiency/D");

    std::vector<int> M_values = {1, 2, 3, 4}; // triggered sensor threshold
    std::vector<int> N_values = {1, 2, 5, 10, 15, 20}; // sensor photon threshold

    for (int M : M_values) {
        for (int N : N_values) {
            int totalEvents = 0;
            int triggeredEvents = 0;

            for (Long64_t entry = 0; entry < nEntries; ++entry) {
                photonTree->GetEntry(entry);

                bool triggerFired = isTriggered(sensorPositions, photonArrivalTimes, M, N);

                if (triggerFired) {
                    triggeredEvents++;
                }

                totalEvents++;
            }

            double triggerEfficiency = static_cast<double>(triggeredEvents) / totalEvents * 100.0;

            std::cout << "M = " << M << ", N = " << N << ", Trigger Efficiency: " << triggerEfficiency << "%" << std::endl;

            rootM = M;
            rootN = N;
            rootEfficiency = triggerEfficiency;
            efficiencyTree->Fill();
        }
    }

    outputRootFile->Write();
    outputRootFile->Close();

    rootFile->Close();

    return 0;
}