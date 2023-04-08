#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialisation.hh"

int main(int argc, char* argv[]) {
    std::string generator_config;
    std::string detector_config;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if ((arg == "-g" || arg == "--generator") && i + 1 < argc) {
            generator_config = argv[++i];
        }
        else if ((arg == "-d" || arg == "--detector") && i + 1 < argc) {
            detector_config = argv[++i];
        }
        else {
            std::cout << "-- Failed to parse command line arguments" << std::endl;
            return 1;
        }
    }
    std::cout << "-- Parsing arguments done" << std::endl;

    auto run_manager = new G4RunManager();
    
    run_manager->SetUserInitialization(new DetectorConstruction(detector_config));
    run_manager->SetUserInitialization(new PhysicsList());
    run_manager->SetUserInitialization(new ActionInitialisation());
    run_manager->Initialize();

    std::ifstream generator_config_stream(generator_config);
    if (generator_config_stream.good()) {
        G4UImanager* ui_manager = G4UImanager::GetUIpointer();
        ui_manager->ApplyCommand("/control/execute " + generator_config);
        std::cout << "-- Macro complete!" << std::endl;
    }
    else {
        std::cout << "-- Failed to open generator config file" << std::endl;
        return 1;
    }

    delete run_manager;

    return 0;
}