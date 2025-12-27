#include "settings_manager.h"

namespace gearforge {

SettingsManager::SettingsManager() {
    load();
}

void SettingsManager::load() {
    std::lock_guard<std::mutex> lock(mutex);
    data = read_ini(filename);
}

void SettingsManager::save() {
    std::lock_guard<std::mutex> lock(mutex);
    std::ofstream file(filename);
    for (auto pair : data) {
        file << pair.first << " = " << pair.second << std::endl;
    }
}

void SettingsManager::add_setting(std::string& line) {
    line = utils::trim(line);
    if (line.empty()) {
        return;
    } else {
        const size_t p = line.find(':');
        if (p == std::string::npos) {
            data[line] = "";
        } else {
            std::string key = utils::trim(line.substr(0, p));
            std::string value = utils::trim(line.substr(p + 1));
            data[key] = value;
        }
    }
}

std::unordered_map<std::string, std::string> SettingsManager::read_ini(const std::string& filename) {
    std::unordered_map<std::string, std::string> data;
    std::ifstream file(filename);

    if (!file) return data;

    std::string line;
    while (std::getline(file, line)) {
        line = utils::trim(line);
        if (line.empty() || line[0] == ';') {
                continue;
        } else {
            const size_t p = line.find('=');
            if (p == std::string::npos) {
                data[line] = "";
            } else {
                std::string key = utils::trim(line.substr(0, p));
                std::string value = utils::trim(line.substr(p + 1));
                data[key] = value;
            }
        }
    }
    return data;
}

}  // namespace gearforge
