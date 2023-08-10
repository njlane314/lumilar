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
                 const std::vector<int>* photonSensorCounts,
                 int M, int N) {       
    std::vector<int> photonsCollected(sensorPositions.size(), 0);

    int triggeredSensorCount = 0;
    for (int opt_det = 0; opt_det < sensorPositions.size(); ++opt_det) {
        int nPhotons = photonSensorCounts->at(opt_det);
        if (nPhotons >= N) {
            triggeredSensorCount++;
        }
    }

    return triggeredSensorCount >= M;
}

double calculateTriggerEfficiencyUncertainty(double efficiency, double totalEvents) {
    if (efficiency < 0.0 || efficiency > 1.0 || totalEvents <= 0.0) {
        std::cerr << "Invalid input values for efficiency or totalEvents." << std::endl;
        return -1.0; 
    }
    
    double uncertainty = std::sqrt((efficiency * (1.0 - efficiency)) / totalEvents);
    return uncertainty;
}

int main(int argc, char* argv[]) {
    std::string placementFileName = "/home/lane/Software/src/lumilar/system/detector/placements/dune_solar_placements.txt";

    TFile* outputRootFile = TFile::Open("../data/position_efficiencies.root", "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file trigger_efficiencies.root" << std::endl;
        return 1;
    }

    outputRootFile->cd();

    TTree* efficiencyTree = new TTree("trigger_efficiencies_tree", "Trigger Efficiencies");

    int rM, rN;
    double rEffic, rEfficError, rDriftDistance;

    efficiencyTree->Branch("M", &rM, "M/I");
    efficiencyTree->Branch("N", &rN, "N/I");
    efficiencyTree->Branch("Efficiency", &rEffic, "Efficiency/D");
    efficiencyTree->Branch("EfficiencyError", &rEfficError, "Efficiency Error/D");
    efficiencyTree->Branch("DriftDistance", &rDriftDistance, "DriftDistance/D");

    std::vector<int> M_values = {4}; // triggered sensor threshold
    std::vector<int> N_values = {10, 15, 30, 50}; // sensor photon threshold

    std::string BIN_PATH = "/home/lane/Software/src/lumilar/system/data/";
    std::vector<std::string> rootFileNames = {
        BIN_PATH + "marley_mono_8_drift_pos1700.root",
        BIN_PATH + "marley_mono_8_drift_pos1600.root",
        BIN_PATH + "marley_mono_8_drift_pos1500.root",
        BIN_PATH + "marley_mono_8_drift_pos1400.root",
        BIN_PATH + "marley_mono_8_drift_pos1300.root",
        BIN_PATH + "marley_mono_8_drift_pos1200.root",
        BIN_PATH + "marley_mono_8_drift_pos1100.root",
        BIN_PATH + "marley_mono_8_drift_pos1000.root",
        BIN_PATH + "marley_mono_8_drift_pos900.root",
        BIN_PATH + "marley_mono_8_drift_pos800.root",
        BIN_PATH + "marley_mono_8_drift_pos700.root",
        BIN_PATH + "marley_mono_8_drift_pos600.root",
        BIN_PATH + "marley_mono_8_drift_pos500.root",
        BIN_PATH + "marley_mono_8_drift_pos400.root",
        BIN_PATH + "marley_mono_8_drift_pos300.root",
        BIN_PATH + "marley_mono_8_drift_pos200.root",
        BIN_PATH + "marley_mono_8_drift_pos100.root",
        BIN_PATH + "marley_mono_8_drift_pos0.root",
        BIN_PATH + "marley_mono_8_drift_neg100.root",
        BIN_PATH + "marley_mono_8_drift_neg200.root",
        BIN_PATH + "marley_mono_8_drift_neg300.root",
    };

    for (const std::string& rootFileName : rootFileNames) {
        TFile* rootFile = TFile::Open(rootFileName.c_str(), "READ");
        if (!rootFile) {
            std::cerr << "-- Error: Cannot open root file " << rootFileName << std::endl;
            return 1;
        }
        std::vector<std::tuple<double, double, double>> sensorPositions = readPlacementFile(placementFileName);

        TTree* photonTree = (TTree*)rootFile->Get("arrival_photons_tree");
        if (!photonTree) {
            std::cerr << "-- Error: Cannot find the photonTree in root file" << std::endl;
            rootFile->Close();
            return 1;
        }

        TTree* marleyTree = (TTree*)rootFile->Get("marley_generator_tree");
        if (!marleyTree) {
            std::cerr << "-- Error: Cannot find the marleyTree in root file" << std::endl;
            rootFile->Close();
            return 1;
        }

        Long64_t nEntries = photonTree->GetEntries();
        std::cout << "Number of entries in the root file: " << nEntries << std::endl;

        std::vector<int> *photonSensorCounts = nullptr;
        double driftDistance = 0.0;

        photonTree->SetBranchAddress("sensor_count", &photonSensorCounts);
        marleyTree->SetBranchAddress("marley_generator_interaction_vertex_x", &driftDistance);

        for (int M : M_values) {
            for (int N : N_values) {
                int totalEvents = 0;
                int triggeredEvents = 0;

                for (Long64_t entry = 1; entry < nEntries; ++entry) {
                    photonTree->GetEntry(entry);
                    marleyTree->GetEntry(entry);

                    bool triggerFired = isTriggered(sensorPositions, photonSensorCounts, M, N);

                    if (triggerFired) {
                        triggeredEvents++;
                    }

                    totalEvents++;
                }

                double triggerEfficiency = static_cast<double>(triggeredEvents) / totalEvents;
                double efficiencyError = calculateTriggerEfficiencyUncertainty(triggerEfficiency, totalEvents);

                std::cout << "M = " << M << ", N = " << N << ", Trigger Efficiency: " << triggerEfficiency << "%" << std::endl;

                rM = M;
                rN = N;
                rEffic = triggerEfficiency;
                rEfficError = efficiencyError;
                rDriftDistance = driftDistance;

                efficiencyTree->Fill();
            }
        }

        rootFile->Close();
    }

    outputRootFile->Write();
    outputRootFile->Close();

    return 0;
}