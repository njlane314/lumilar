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

double calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1));
}

void calculateConcentricDistances(const std::vector<std::tuple<double, double, double>>& sensorPositions,
                               const std::tuple<double, double, double>& centralDetector,
                               std::vector<double>& concentricDistances) {
    std::cout << "Calculating concentric layers." << std::endl;
    for (int i = 0; i < sensorPositions.size(); ++i) {
        double x, y, z;
        std::tie(x, y, z) = sensorPositions[i];
        double distance = calculateDistance(x, y, z, std::get<0>(centralDetector),
                                            std::get<1>(centralDetector), std::get<2>(centralDetector));
        if (distance < 1000) {
            concentricDistances.push_back(distance);
        }

        std::cout << distance.size() << std::endl;
        std::sort(concentricDistances.begin(), concentricDistances.end());
    }
}

double calculateTriggerEfficiencyUncertainty(double efficiency, double totalEvents) {
    if (efficiency < 0.0 || efficiency > 1.0 || totalEvents <= 0.0) {
        std::cerr << "-- Error: Invalid input values for efficiency or totalEvents." << std::endl;
        return -1.0; 
    }
    
    double uncertainty = std::sqrt((efficiency * (1.0 - efficiency)) / totalEvents);
    return uncertainty;
}

int numberOfSquaresInLayer(int n) {
    return 8 * n;
}

void processConcentricDistances(const std::vector<std::tuple<double, double, double>>& sensorPositions,
                            const std::vector<int>& photonSensorCounts,
                            const int N_THRESHOLD) {
    int maxSensorCount = 0;
    int maxSensorIndex = -1;

    for (int opt_det = 0; opt_det < sensorPositions.size(); ++opt_det) {
        int nPhotons = photonSensorCounts[opt_det];
        if (nPhotons > maxSensorCount && nPhotons > N_THRESHOLD) {
            maxSensorCount = nPhotons;
            maxSensorIndex = opt_det;
        }
    }

    if (maxSensorIndex == -1) {
        std::cout << "-- Error: No sensor with photons found." << std::endl;
        return;
    }

    std::vector<double> concentricDistances(sensorPositions.size());
    calculateConcentricDistances(sensorPositions, sensorPositions[maxSensorIndex], concentricDistances);

    std::vector<std::vector<double>> layers;
    for (int distanceIndex = 0; distanceIndex < concentricDistances.size(); ++distanceIndex) {
        int layer = 1;
        while (distanceIndex < numberOfSquaresInLayer(layer)) {
            ++layer;
        }

        if (layer >= layers.size()) {
            layers.resize(layer + 1);
        }

        layers[layer].push_back(concentricDistances[distanceIndex]);
    }

    for (int layer = 0; layer < layers.size(); ++layer) {
        std::cout << "Layer " << layer << ": ";
        for (double distance : layers[layer]) {
            std::cout << distance << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string placementFileName = "/home/lane/Software/src/lumilar/system/detector/placements/dune_solar_placements.txt";

    TFile* outputRootFile = TFile::Open("../data/active_fiducialisation.root", "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file active_fiducialistion.root" << std::endl;
        return 1;
    }

    outputRootFile->cd();

    TTree* efficiencyTree = new TTree("active_fiducialisation_tree", "Active Fiducialisation Tree");

    int rN;
    double rEffic, rEfficError, rEnergy;

    efficiencyTree->Branch("N", &rN, "N/I");
    efficiencyTree->Branch("Efficiency", &rEffic, "Efficiency/D");
    efficiencyTree->Branch("EfficiencyError", &rEfficError, "Efficiency Error/D");
    efficiencyTree->Branch("Energy", &rEnergy, "Energy/D");

    int N_THRESHOLD = 100;

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

    std::vector<std::tuple<double, double, double>> sensorPositions = readPlacementFile(placementFileName);

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

        std::vector<int> *photonSensorCounts = nullptr;
        photonTree->SetBranchAddress("sensor_count", &photonSensorCounts);

        Long64_t nEntries = photonTree->GetEntries();
        std::cout << "Number of entries in the root file: " << nEntries << std::endl;

        for (Long64_t entry = 1; entry < nEntries; ++entry) {
            photonTree->GetEntry(entry);
            std::cout << "Processing entry " << entry << std::endl;
            processConcentricDistances(sensorPositions, *photonSensorCounts, N_THRESHOLD);
        }
    }

    outputRootFile->Close();

    return 0;
}