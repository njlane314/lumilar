#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMath.h"
#include "TProfile.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "-- Usage: " << argv[0] << " <input filename>" << endl;
        return 1;
    }

    cout << "-- Running with: " << argv[1] << endl;

    TFile* input_file = new TFile(argv[1]);

    if (!input_file || input_file->IsZombie()) {
        cout << "-- Error opening file: " << argv[1] << endl;
        return 1;
    }

    cout << "-- File opened successfully" << endl;

    TTree* detector_tree = (TTree*)input_file->Get("detector_tree_");

    if (!detector_tree) {
        cout << "-- Error: Tree 'detector_tree_' not found in file" << endl;
        return 1;
    }

    int total_thermal_electrons;
    detector_tree->SetBranchAddress("total_thermal_electrons", &total_thermal_electrons);

    int total_optical_photons;
    detector_tree->SetBranchAddress("total_optical_photons", &total_optical_photons);

    int num_events = detector_tree->GetEntries();

    TH2D* hist = new TH2D("hist", "",
                          100, 1, 0, 100, 1, 0);

    for (int i = 0; i < num_events; i++) {
        detector_tree->GetEntry(i);
        cout << "Total thermal electrons : " << total_thermal_electrons << endl;
        cout << "Total optical photons : " << total_optical_photons << endl;
        hist->Fill(total_thermal_electrons, total_optical_photons);
    }

    TCanvas* canvas = new TCanvas("canvas", "2D Histogram", 800, 600);
    canvas->SetMargin(0.1, 0.1, 0.1, 0.1);

    hist->Draw("colz");

    hist->GetXaxis()->SetTitle("Total Thermal Electron");
    hist->GetYaxis()->SetTitle("Total Optical Photon");

    canvas->Update();

    canvas->SaveAs("plots/charge_light_signals.png");

    delete canvas;
    delete hist;
    delete input_file;

    return 0;
}