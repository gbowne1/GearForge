#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <glog/logging.h>  // 2nd party

namespace gearforge {
namespace utils {

// ANSI escapes
static const std::string CLEAR_SCREEN = "\033[2J\033[H";
static const std::string COLOR_BLACK = "\033[30m";
static const std::string COLOR_WHITE = "\033[37m";
static const std::string COLOR_BLUE = "\033[34m";
static const std::string COLOR_GRAY = "\033[90m";
static const std::string COLOR_YELLOW = "\033[33m";
static const std::string COLOR_RED = "\033[31m";
static const std::string COLOR_GREEN = "\033[32m";
static const std::string COLOR_RESET = "\033[0m";
static const std::string INVERSE_ON = "\033[7m";
static const std::string INVERSE_OFF = "\033[27m";

// Box drawing (Unicode)
static const std::string BOX_TOP_LEFT = "┌";
static const std::string BOX_TOP_RIGHT = "┐";
static const std::string BOX_BOTTOM_LEFT = "└";
static const std::string BOX_BOTTOM_RIGHT = "┘";
static const std::string BOX_HORIZONTAL = "─";
static const std::string BOX_VERTICAL = "│";

// Custom SHA256
struct Sha256Hash {
    std::array<uint32_t, 8> state;
};
Sha256Hash sha256(const std::string& input);  // Returns hash state; use to_string on digest

// Progress bar
void show_progress(int progress, int total, const std::string& label);

// File utils
bool file_exists(const std::filesystem::path& p);
std::vector<std::vector<std::string>> read_csv(const std::string& filename);
bool write_csv(const std::string& filename, const std::vector<std::vector<std::string>>& data);

// String utils
std::string trim(const std::string& str);
std::string to_lower(const std::string& str);

// Input utils
char get_key();  // For WASD/arrow handling (uses system("stty raw"))

// Time utils
std::string current_date();

// Other reusables...
double safe_stod(const std::string& str);

}  // namespace utils
}  // namespace gearforge
