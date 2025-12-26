#pragma once

#include "utils.h"

namespace gearforge {

class SettingsManager {
private:
    std::mutex mutex;
    std::string filename = "data/settings.ini";

    std::unordered_map<std::string, std::string> read_ini(const std::string& filename);
    void load();

public:
    SettingsManager();
    std::unordered_map<std::string, std::string> data;
    void save();
    void add_setting(std::string& line);
};

}  // namespace gearforge
