#pragma once

#include "utils.h"

namespace gearforge {

struct GearParams {
    int n;             // Number of teeth
    double dp;         // Diametrical Pitch
    double m;          // Module (metric)
    double pd;         // Pitch Diameter
    double od;         // Outside Diameter
    double rd;         // Root Diameter
    double a;          // Addendum
    double d;          // Dedendum
    double wd;         // Whole Depth
    double cp;         // Circular Pitch
    double pa;         // Pressure Angle (degrees)
    double cd;         // Center Distance (for pair)
    double backlash;   // Backlash

    // Serialize to vector for CSV
    std::vector<std::string> to_csv_row() const;
    static GearParams from_csv_row(const std::vector<std::string>& row);
};

class GearCalculator {
public:
    GearCalculator() = default;

    // Calculate all from minimal inputs (e.g., N, DP or M, PA)
    GearParams calculate(const GearParams& input);

    // Select cutter: Returns cutter number (1-8 for standard involute)
    int select_cutter(int teeth);

    // Dividing head instructions for 40:1
    std::string dividing_head_instructions(int teeth);

    // Involute points (parametric, theta in radians)
    std::pair<double, double> involute_point(double r_base, double theta);

    // Load known values from CSV
    std::vector<GearParams> load_known(const std::string& filename);

    // Save to CSV
    bool save(const GearParams& params, const std::string& filename);
};

}  // namespace gearforge
