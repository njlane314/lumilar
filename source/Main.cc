#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <filesystem>
//_________________________________________________________________________________________
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
//_________________________________________________________________________________________
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "PhysicsList.hh"
#include "ActionInitialisation.hh"
#include "AnalysisManager.hh"
//_________________________________________________________________________________________
int main(int argc, char* argv[]) {
    std::cout << "\n       :::       :::    :::   :::   :::   ::::::::::: :::            :::     ::::::::: \n"
                 "     :+:       :+:    :+:  :+:+: :+:+:      :+:     :+:          :+: :+:   :+:    :+: \n"
                 "    +:+       +:+    +:+ +:+ +:+:+ +:+     +:+     +:+         +:+   +:+  +:+    +:+  \n"
                 "   +#+       +#+    +:+ +#+  +:+  +#+     +#+     +#+        +#++:++#++: +#++:++#:   \n"
                 "  +#+       +#+    +#+ +#+       +#+     +#+     +#+        +#+     +#+ +#+    +#+   \n"
                 " #+#       #+#    #+# #+#       #+#     #+#     #+#        #+#     #+# #+#    #+#    \n"
                 "########## ########  ###       ### ########### ########## ###     ### ###    ###    \n";
    
    std::cout << "\n\n**************************************************************\n";
    std::cout << "Version: 0.0\n"
              << "Author(s): Nicholas Lane (nicholas.lane@postgrad.manchester.ac.uk)\n"
              << "Date version: 20-05-23\n";
    std::cout << "**************************************************************\n\n";	

    std::cout << "-- Starting program..." << std::endl;     

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

    auto now = std::chrono::system_clock::now();
    std::stringstream now_stream;
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    now_stream << std::put_time(std::gmtime(&now_time_t), "%Y-%m-%dT%H:%M:%SZ");

    std::string output_filename;
    if (!generator_config.empty()) {
        auto last_slash_pos = generator_config.find_last_of("/\\");
        std::string generator_filename = generator_config.substr(last_slash_pos + 1);
        std::string generator_config_filename_without_ext = std::filesystem::path(generator_filename).stem();

        output_filename = "data/" + now_stream.str() + "_" + generator_config_filename_without_ext + ".root";
    } else {
        output_filename = now_stream.str();
    }
    
    std::cout << "-- Set output filename to " << output_filename << std::endl;

    auto run_manager = new G4RunManager();
    run_manager->SetUserInitialization(new DetectorConstruction(new DetectorMessenger()));

    std::ifstream detector_config_stream(detector_config);
    if (detector_config_stream.good()) {
        G4UImanager* ui_manager = G4UImanager::GetUIpointer();
        ui_manager->ApplyCommand("/control/execute " + detector_config);
        std::cout << "-- Detector macro complete!" << std::endl;
    }
    else {
        std::cout << "-- Failed to open detetcor macro..." << std::endl;
        return 1;
    }

    
    run_manager->SetUserInitialization(new PhysicsList());
    run_manager->SetUserInitialization(new ActionInitialisation(output_filename));
    run_manager->Initialize();

    AnalysisManager::GetInstance()->SetOutputFilename(output_filename);

    std::cout << "-- Initialisation complete!" << std::endl;

    std::ifstream generator_config_stream(generator_config);
    if (generator_config_stream.good()) {
        G4UImanager* ui_manager = G4UImanager::GetUIpointer();
        ui_manager->ApplyCommand("/control/execute " + generator_config);
        std::cout << "-- Macro complete!" << std::endl;
    }
    else {
        std::cout << "-- Failed to open generator macro..." << std::endl;
        return 1;
    }
    
    delete run_manager;

    return 0;
}
