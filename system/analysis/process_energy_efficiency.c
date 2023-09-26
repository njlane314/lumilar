#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <map>
#include <algorithm>
#include <set>
#include "TFile.h"
#include "TTree.h"
#include <numeric>

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

    TFile* outputRootFile = TFile::Open("../data/trigger_efficiencies.root", "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file trigger_efficiencies.root" << std::endl;
        return 1;
    }

    outputRootFile->cd();

    TTree* efficiencyTree = new TTree("trigger_efficiencies_tree", "Trigger Efficiencies");
    TTree* efficiencyEventTree = new TTree("event_tree", "Event Tree");

    std::vector<int> rSumCounts;
    std::vector<int> rThermalElectrons;

    efficiencyTree->Branch("SumCounts", &rSumCounts);
    efficiencyTree->Branch("ThermalElectrons", &rThermalElectrons);


    int rM, rN;
    double rEffic, rEfficError, rEnergy;
    int rSensorSum, rNThermalElectrons;

    efficiencyTree->Branch("M", &rM, "M/I");
    efficiencyTree->Branch("N", &rN, "N/I");
    efficiencyTree->Branch("Efficiency", &rEffic, "Efficiency/D");
    efficiencyTree->Branch("EfficiencyError", &rEfficError, "Efficiency Error/D");
    efficiencyTree->Branch("Energy", &rEnergy, "Energy/D");

    efficiencyEventTree->Branch("SensorSum", &rSensorSum, "SensorSum/I");
    efficiencyEventTree->Branch("NThermalElectrons", &rNThermalElectrons, "NThermalElectrons/I");
    efficiencyEventTree->Branch("Energy", &rEnergy, "Energy/D");

    std::vector<int> M_values = {1}; 
    std::vector<int> N_values = {4, 10, 20, 50, 100};

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

    std::vector<std::tuple<double, double, double>> sensorPositions = readPlacementFile(placementFileName);

    int nuEnergy = 4;
    for (const std::string& rootFileName : rootFileNames) {
        TFile* rootFile = TFile::Open(rootFileName.c_str(), "READ");
        if (!rootFile) {
            std::cerr << "-- Error: Cannot open root file " << rootFileName << std::endl;
            continue;
        }

        TTree* photonTree = (TTree*)rootFile->Get("arrival_photons_tree");
        if (!photonTree) {
            std::cerr << "-- Error: Cannot find the photonTree in root file" << std::endl;
            rootFile->Close();
            continue;
        }

        TTree* eventTree = (TTree*)rootFile->Get("event_tree");
        if (!eventTree) {
            std::cerr << "-- Error: Cannot find the eventTree in root file" << std::endl;
            rootFile->Close();
            continue;
        }

        Long64_t nEntries = photonTree->GetEntries();
        std::cout << "Number of entries in the root file: " << nEntries << std::endl;

        std::vector<int> *photonSensorCounts = nullptr;
        int nThermalElectrons = 0;

        eventTree->SetBranchAddress("total_thermal_electrons", &nThermalElectrons);
        photonTree->SetBranchAddress("sensor_count", &photonSensorCounts);

        for (int M : M_values) {
            for (int N : N_values) {
                int totalEvents = 0;
                int triggeredEvents = 0;

                rSumCounts.clear();
                rThermalElectrons.clear();

                for (Long64_t entry = 1; entry < nEntries; ++entry) {
                    photonTree->GetEntry(entry);
                    eventTree->GetEntry(entry);

                    int currentSensorSum = std::accumulate(photonSensorCounts->begin(), photonSensorCounts->end(), 0);
                    bool triggerFired = isTriggered(sensorPositions, photonSensorCounts, M, N);

                    if (triggerFired) {
                        rSumCounts.push_back(currentSensorSum);
                        rThermalElectrons.push_back(nThermalElectrons);
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
                rEnergy = nuEnergy;

                efficiencyTree->Fill();
            }
        }

        rootFile->Close();
        nuEnergy++;
    }

    outputRootFile->Write();
    outputRootFile->Close();

    return 0;
}