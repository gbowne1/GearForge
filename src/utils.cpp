#include "utils.h"

namespace gearforge {
namespace utils {

Sha256Hash sha256(const std::string& input) {
    // Standard SHA256 implementation (omitted for brevity; ~200 lines).
    // Initialize with constants, process blocks, etc.
    // Use uint32_t rotations, etc. No inline asm needed here.
    Sha256Hash hash;
    // ... compute ...
    return hash;
}

void show_progress(int progress, int total, const std::string& label) {
    int bar_width = 50;
    float frac = static_cast<float>(progress) / total;
    int width = static_cast<int>(frac * bar_width);
    std::cout << label << " [" << COLOR_GREEN;
    for (int i = 0; i < width; ++i) std::cout << "=";
    std::cout << ">" << COLOR_RESET;
    for (int i = width; i < bar_width; ++i) std::cout << " ";
    std::cout << "] " << std::setw(3) << static_cast<int>(frac * 100) << "%\r" << std::flush;
}

bool file_exists(const std::filesystem::path& p) {
    return std::filesystem::exists(p);
}

std::vector<std::vector<std::string>> read_csv(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    if (!file) return data;
    std::string line;
    int row = 0;
    show_progress(0, 100, "Loading CSV");  // Dummy progress
    while (std::getline(file, line)) {
        std::vector<std::string> row_data;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row_data.push_back(trim(cell));
        }
        data.push_back(row_data);
        show_progress(++row % 100, 100, "Loading CSV");
    }
    std::cout << std::endl;
    return data;
}

bool write_csv(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename);
    if (!file) return false;
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }
    return true;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

std::string to_lower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

char get_key() {
    system("stty raw -echo");
    char c = std::cin.get();
    system("stty cooked echo");
    if (c == 27) {  // Escape for arrows
        c = std::cin.get();  // [
        c = std::cin.get();  // A=up, B=down, etc.
        switch (c) {
            case 'A': return 'w';  // Map to WASD
            case 'B': return 's';
            case 'C': return 'd';
            case 'D': return 'a';
        }
    }
    return tolower(c);  // Support ijkl too via mapping if needed
}

std::string current_date() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", local);
    return buf;
}

double safe_stod(const std::string& str) {
    try {
        return std::stod(str);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Invalid number: " + str);
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Number out of range: " + str);
    }
}

}  // namespace utils
}  // namespace gearforge
