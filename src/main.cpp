#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <glog/logging.h>
#include <gtest/gtest.h>

#include "gear_calculator.h"
#include "ui.h"
#include "user_manager.h"
#include "utils.h"

using namespace gearforge;

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);

    // Command-line flags
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            std::cout << "Usage: gearforge [--version] [--load=file.csv]" << std::endl;
            return 0;
        } else if (arg == "--version") {
            std::cout << "GearForge v0.0.1" << std::endl;
            return 0;
        } else if (arg.find("--load=") == 0) {
            // Load CSV; handle in UI
        }
    }

    // Ensure data dir
    std::filesystem::create_directory("data");

    UserManager um;
    Ui ui(um);
    ui.run();

    return 0;
}
