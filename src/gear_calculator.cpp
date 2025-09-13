#include "gear_calculator.h"

namespace gearforge {

std::vector<std::string> GearParams::to_csv_row() const {
    return {
        std::to_string(n), std::to_string(dp), std::to_string(m),
        std::to_string(pd), std::to_string(od), std::to_string(rd),
        std::to_string(a), std::to_string(d), std::to_string(wd),
        std::to_string(cp), std::to_string(pa), std::to_string(cd),
        std::to_string(backlash)
    };
}

GearParams GearParams::from_csv_row(const std::vector<std::string>& row) {
    if (row.size() < 13) throw std::runtime_error("Invalid CSV row for GearParams");
    GearParams p;
    p.n = std::stoi(row[0]);
    p.dp = utils::safe_stod(row[1]);
    p.m = utils::safe_stod(row[2]);
    p.pd = utils::safe_stod(row[3]);
    p.od = utils::safe_stod(row[4]);
    p.rd = utils::safe_stod(row[5]);
    p.a = utils::safe_stod(row[6]);
    p.d = utils::safe_stod(row[7]);
    p.wd = utils::safe_stod(row[8]);
    p.cp = utils::safe_stod(row[9]);
    p.pa = utils::safe_stod(row[10]);
    p.cd = utils::safe_stod(row[11]);
    p.backlash = utils::safe_stod(row[12]);
    return p;
}

GearParams GearCalculator::calculate(const GearParams& input) {
    GearParams p = input;
    if (std::isnan(p.dp) && !std::isnan(p.m)) p.dp = 25.4 / p.m;  // Convert module to DP
    if (std::isnan(p.m) && !std::isnan(p.dp)) p.m = 25.4 / p.dp;
    p.pd = p.n / p.dp;
    p.a = 1.0 / p.dp;
    p.d = 1.157 / p.dp;  // Standard for 14.5/20 deg PA
    p.wd = p.a + p.d;
    p.od = p.pd + 2 * p.a;
    p.rd = p.pd - 2 * p.d;
    p.cp = M_PI / p.dp;
    if (std::isnan(p.cd)) p.cd = p.pd / 2.0;  // Single gear; for pair, user input
    if (std::isnan(p.backlash)) p.backlash = 0.003 * p.pd;  // Mid-range default
    // Adjust for PA if non-standard, but assume 20 deg default if nan
    if (std::isnan(p.pa)) p.pa = 20.0;
    return p;
}

int GearCalculator::select_cutter(int teeth) {
    // Standard involute cutter ranges
    if (teeth >= 135) return 1;
    if (teeth >= 55) return 2;
    if (teeth >= 35) return 3;
    if (teeth >= 26) return 4;
    if (teeth >= 21) return 5;
    if (teeth >= 17) return 6;
    if (teeth >= 14) return 7;
    return 8;  // 12-13
}

std::string GearCalculator::dividing_head_instructions(int teeth) {
    double turns = 40.0 / teeth;
    int full_turns = static_cast<int>(turns);
    double fractional = turns - full_turns;
    // Assume standard hole plates; simplify
    return "For 40:1 dividing head: " + std::to_string(full_turns) + " full turns + " +
           std::to_string(fractional) + " fractional (use hole plate).";
}

std::pair<double, double> GearCalculator::involute_point(double r_base, double theta) {
    double x = r_base * (std::cos(theta) + theta * std::sin(theta));
    double y = r_base * (std::sin(theta) - theta * std::cos(theta));
    return {x, y};
}

std::vector<GearParams> GearCalculator::load_known(const std::string& filename) {
    auto csv = utils::read_csv(filename);
    std::vector<GearParams> params;
    for (size_t i = 1; i < csv.size(); ++i) {  // Skip header
        params.push_back(GearParams::from_csv_row(csv[i]));
    }
    return params;
}

bool GearCalculator::save(const GearParams& params, const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    // Header
    data.push_back({"N", "DP", "M", "PD", "OD", "RD", "A", "D", "WD", "CP", "PA", "CD", "Backlash"});
    data.push_back(params.to_csv_row());
    return utils::write_csv(filename, data);
}

}  // namespace gearforge
