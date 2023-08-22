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
    TFile* outputRootFile = TFile::Open("../data/radiological_backgrounds.root", "RECREATE");
    if (!outputRootFile) {
        std::cerr << "-- Error: Cannot create output ROOT file trigger_efficiencies.root" << std::endl;
        return 1;
    }

}