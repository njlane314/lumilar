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
#include "TGraphErrors.h"
#include "TMath.h"
#include "TProfile.h"

using namespace std;

int main(int argc, char* argv[]) {
    double DELAYED_ENERGY_LEVEL = 1.64364;
    int NUM_BINS = 19;
    int E_MIN = 0;
    int E_MAX = 20;

    if (argc < 2) {
        cout << "-- Usage: " << argv[0] << " <input filename>" << endl;
        return 1;
    }

    cout << "-- Running with: " << argv[1] << endl;

    TFile* input_file = new TFile(argv[1]);

    cout << "-- File opened successfully" << endl;

    TTree* cascade_level_tree = (TTree*)input_file->Get("cascade_level_tree_");
    std::vector<double>* cascade_levels = nullptr;
    cascade_level_tree->SetBranchAddress("cascade_levels", &cascade_levels);

    TTree* detector_tree = (TTree*)input_file->Get("detector_tree_");
    double neutrino_energy;
    detector_tree->SetBranchAddress("primary_energy", &neutrino_energy);

    TH1D* full_energy_hist = new TH1D("full_energy_hist", "Neutrino Energy", NUM_BINS, E_MIN, E_MAX);
    TH1D* delayed_energy_hist = new TH1D("delayed_energy_hist", "Neutrino Energy", NUM_BINS, E_MIN, E_MAX);

    int num_events = cascade_level_tree->GetEntries();
    for (int i = 0; i < num_events; i++) {
        cascade_level_tree->GetEntry(i);
        detector_tree->GetEntry(i);
        for (auto j = 0; j < cascade_levels->size(); j++) {
            cout << "Cascade Level: " << cascade_levels->at(j) << endl;
            cout << "Neutrino Energy: " << neutrino_energy << endl;

            if (cascade_levels->at(j) == DELAYED_ENERGY_LEVEL) {
                delayed_energy_hist->Fill(neutrino_energy);
            }
        }
        full_energy_hist->Fill(neutrino_energy);
    }

    TCanvas* canvas = new TCanvas("canvas", "Energy Histograms", 1600, 1200);
    canvas->SetMargin(0.1, 0.1, 0.1, 0.1);
    canvas->SetGrid();

    TGraphErrors* fraction_graph = new TGraphErrors();
    fraction_graph->SetName("fraction_graph");
    fraction_graph->SetMarkerStyle(20);
    fraction_graph->SetMarkerSize(1.0);
    fraction_graph->SetMarkerColor(kBlue);
    fraction_graph->SetLineColor(kBlue);

    for (int bin = 2; bin <= full_energy_hist->GetNbinsX(); bin++) {
        double full_entries = full_energy_hist->GetBinContent(bin);
        double delayed_entries = delayed_energy_hist->GetBinContent(bin);
        double fraction = (delayed_entries > 0) ? delayed_entries / full_entries : 0;

        double full_entries_error = sqrt(full_entries);
        double delayed_entries_error = sqrt(delayed_entries);

        fraction_graph->SetPoint(bin - 2, full_energy_hist->GetBinCenter(bin), fraction);
        fraction_graph->SetPointError(bin - 2, 0, delayed_entries_error / full_entries);
    }

    fraction_graph->GetXaxis()->SetTitle("Neutrino Energy [MeV]");
    fraction_graph->GetXaxis()->SetTitleSize(0.04);
    fraction_graph->GetXaxis()->SetTitleFont(42);
    fraction_graph->GetXaxis()->SetLabelSize(0.035);
    fraction_graph->GetXaxis()->SetLabelFont(42);

    fraction_graph->GetYaxis()->SetTitle("Fraction of Events with 0^{+} Cascade Level");
    fraction_graph->GetYaxis()->SetTitleSize(0.04);
    fraction_graph->GetYaxis()->SetTitleFont(42);
    fraction_graph->GetYaxis()->SetLabelSize(0.035);
    fraction_graph->GetYaxis()->SetLabelFont(42);

    fraction_graph->Draw("AP");
    fraction_graph->GetHistogram()->GetXaxis()->SetRangeUser(E_MIN, E_MAX);
    //fraction_graph->GetHistogram()->GetYaxis()->SetRangeUser(0, 1);

    canvas->SaveAs("plots/fraction_profile.png");

    delete fraction_graph;
    delete full_energy_hist;
    delete delayed_energy_hist;

    delete detector_tree;
    delete cascade_level_tree;

    input_file->Close();

    delete input_file;
    delete canvas;

    return 0;
}



