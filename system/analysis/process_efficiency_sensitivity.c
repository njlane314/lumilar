#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <map>
#include <algorithm>
#include <set>
#include "TFile.h"
#include "TTree.h"

struct CandidateEvent {
    std::string trueLabel; 
    bool triggerFired;    
};

double calculateF1Score(double TP, double FP, double FN) {
    double precision = (TP) / (TP + FP);
    double recall = (TP) / (TP + FN);

    if ((precision + recall) == 0) { 
        return 0.0;
    }

    return 2 * (precision * recall) / (precision + recall);
}

const double PROMPT_WINDOW_START = 0.1; 
const double PROMPT_WINDOW_END = 90.0;  
const double FPROMPT_THRESHOLD = 0.3;   

bool isPrompt(double time) {
    return time >= PROMPT_WINDOW_START && time <= PROMPT_WINDOW_END;
}

bool isTriggered(const std::vector<int>* photonSensorCounts, const std::vector<std::vector<double>>* photonSensorTimes, int M, int N) {
    int triggeredSensorCount = 0;
    std::vector<double> allPromptPhotons; 

    for (int opt_det = 0; opt_det < photonSensorCounts->size(); ++opt_det) {
        int nPhotons = photonSensorCounts->at(opt_det);
        if (nPhotons >= N) {
            triggeredSensorCount++;
            allPromptPhotons.insert(allPromptPhotons.end(), photonSensorTimes->at(opt_det).begin(), photonSensorTimes->at(opt_det).end());
        }
    }

    int promptPhotonCount = std::count_if(allPromptPhotons.begin(), allPromptPhotons.end(), isPrompt);
    double fPrompt = static_cast<double>(promptPhotonCount) / allPromptPhotons.size();

    return (triggeredSensorCount >= M) && (fPrompt >= FPROMPT_THRESHOLD);
}

int main(int argc, char* argv[]) {
    std::cout << "Starting the program..." << std::endl;
    TFile* outputRootFile = TFile::Open("../data/precision_sensitivity.root", "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file precision_sensitivity.root" << std::endl;
        return 1;
    }

    outputRootFile->cd();
    TTree* precisionSensitivityTree = new TTree("precision_sensitivity_tree", "Precision Sensitivity Tree");

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

    std::map<std::string, double> signalRates = {
        {BIN_PATH + "marley_B8_CC.root", 1e-10},  
        {BIN_PATH + "marley_HEP_CC.root", 1e-10}  
    };

    std::map<std::string, double> backgroundRates = {
        {BIN_PATH + "bxdecay0_Ar39.root", 1.010},  
        {BIN_PATH + "bxdecay0_Ar42.root", 0.000092},
        {BIN_PATH + "bxdecay0_Bi214.root", 0.000092},
        {BIN_PATH + "bxdecay0_K42.root", 0.01},
        {BIN_PATH + "bxdecay0_Kr85.root", 0.115},
        {BIN_PATH + "bxdecay0_Pb214.root", 0.01},
        {BIN_PATH + "bxdecay0_Rn222.root", 0.04}  
    };

    std::vector<int> MValues = {2, 4, 6, 8};
    std::vector<int> NValues = {5, 10, 20, 50};

    int currentM, currentN;
    double f1Score;
    precisionSensitivityTree->Branch("M", &currentM, "M/I");
    precisionSensitivityTree->Branch("N", &currentN, "N/I");
    precisionSensitivityTree->Branch("F1Score", &f1Score, "F1Score/D");

    for (int M : MValues) {
        for (int N : NValues) {
            std::cout << "Processing for M=" << M << " and N=" << N << std::endl;
            double TP = 0., FP = 0., TN = 0., FN = 0.;

            for (const auto& filename : signalFilenames) {
                std::cout << "Reading signal file " << filename << std::endl;
                double currentRate = signalRates[filename];
                TFile* file = TFile::Open(filename.c_str(), "READ");
                if (!file) {
                    std::cerr << "-- Error: Cannot open file " << filename << std::endl;
                    continue;
                }

                TTree* photonTree = (TTree*)file->Get("arrival_photons_tree");
                if (!photonTree) {
                    std::cerr << "-- Error: Cannot find the photonTree in root file" << std::endl;
                    file->Close();
                    return 1;
                }

                Long64_t nEntries = photonTree->GetEntries();
                std::vector<int>* photonSensorCounts = nullptr;
                photonTree->SetBranchAddress("sensor_count", &photonSensorCounts);
                std::vector<std::vector<double>>* photonSensorTimes = nullptr;
                photonTree->SetBranchAddress("sensor_arrival_times", &photonSensorTimes);

                for (Long64_t entry = 1; entry < nEntries; ++entry) {
                    photonTree->GetEntry(entry);
                    bool triggerFired = isTriggered(photonSensorCounts, photonSensorTimes, M, N);
                    if(triggerFired) {
                        TP += currentRate; 
                    } else {
                        FN += currentRate;
                    }
                }

                file->Close();
                delete file;

                std::cout << "Finished processing signal file " << filename << std::endl;
            }

            for (const auto& filename : backgroundFilenames) {
                std::cout << "Reading background file " << filename << std::endl;
                double currentRate = backgroundRates[filename];
                TFile* file = TFile::Open(filename.c_str(), "READ");
                if (!file) {
                    std::cerr << "-- Error: Cannot open file " << filename << std::endl;
                    continue;
                }

                TTree* photonTree = (TTree*)file->Get("arrival_photons_tree");
                if (!photonTree) {
                    std::cerr << "-- Error: Cannot find the photonTree in root file" << std::endl;
                    file->Close();
                    return 1;
                }

                Long64_t nEntries = photonTree->GetEntries();
                std::vector<int>* photonSensorCounts;
                photonTree->SetBranchAddress("sensor_count", &photonSensorCounts);
                std::vector<std::vector<double>>* photonSensorTimes = nullptr;
                photonTree->SetBranchAddress("sensor_arrival_times", &photonSensorTimes);

                for (Long64_t entry = 1; entry < nEntries; ++entry) {
                    photonTree->GetEntry(entry);
                    bool triggerFired = isTriggered(photonSensorCounts, photonSensorTimes, M, N);
                    if(triggerFired) {
                        FP += currentRate;
                    } else {
                        TN += currentRate;
                    }
                }

                file->Close();
                delete file;

                std::cout << "Finished processing background file " << filename << std::endl;
            }
        
            f1Score = calculateF1Score(TP, FP, FN);
            currentM = M;
            currentN = N;
            precisionSensitivityTree->Fill();

            std::cout << "M=" << M << ", N=" << N << " - F1 Score: " << f1Score << std::endl;
        }
    }

    std::cout << "Writing to the output ROOT file and closing..." << std::endl;

    outputRootFile->Write();
    outputRootFile->Close();
    delete outputRootFile;

    std::cout << "Program completed successfully!" << std::endl;

    return 0;
}