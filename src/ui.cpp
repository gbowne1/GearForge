#include "ui.h"

namespace gearforge {

void Ui::draw_box(const std::string& title, const std::vector<std::string>& lines) {
    size_t max_len = title.length();
    for (const auto& l : lines) max_len = std::max(max_len, l.length());
    max_len += 4;  // Padding

    std::cout << utils::COLOR_BLUE << utils::BOX_TOP_LEFT;
    for (size_t i = 0; i < max_len - 2; ++i) std::cout << utils::BOX_HORIZONTAL;
    std::cout << utils::BOX_TOP_RIGHT << utils::COLOR_RESET << std::endl;

    std::cout << utils::COLOR_BLUE << utils::BOX_VERTICAL << utils::COLOR_RESET << " " << title;
    for (size_t i = title.length() + 2; i < max_len - 1; ++i) std::cout << " ";
    std::cout << utils::COLOR_BLUE << utils::BOX_VERTICAL << utils::COLOR_RESET << std::endl;

    for (const auto& line : lines) {
        std::cout << utils::COLOR_BLUE << utils::BOX_VERTICAL << utils::COLOR_RESET << " " << line;
        for (size_t i = line.length() + 2; i < max_len - 1; ++i) std::cout << " ";
        std::cout << utils::COLOR_BLUE << utils::BOX_VERTICAL << utils::COLOR_RESET << std::endl;
    }

    std::cout << utils::COLOR_BLUE << utils::BOX_BOTTOM_LEFT;
    for (size_t i = 0; i < max_len - 2; ++i) std::cout << utils::BOX_HORIZONTAL;
    std::cout << utils::BOX_BOTTOM_RIGHT << utils::COLOR_RESET << std::endl;
}

void Ui::show_main_screen() {
    std::cout << utils::CLEAR_SCREEN;
    draw_box("GearForge v0.0.1", {"Copyright (c) 2025 Your Name", "MIT License"});
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << utils::CLEAR_SCREEN;
}

bool Ui::show_login_register() {
    std::cout << utils::CLEAR_SCREEN;
    int choice = select_menu({"Login", "Register", "Exit"});
    if (choice == 2) return false;

    std::string username, password;
    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);  // Note: No echo; in real, use getpass but not std

    if (choice == 0) {
        if (user_manager.login(username, password)) {
            LOG(INFO) << "User " << username << " logged in.";
            return true;
        } else {
            handle_error("Login failed.");
            return show_login_register();
        }
    } else {
        if (user_manager.register_user(username, password)) {
            LOG(INFO) << "User " << username << " registered.";
            return true;
        } else {
            handle_error("Registration failed (username exists?).");
            return show_login_register();
        }
    }
}

void Ui::show_main_menu() {
    while (running) {
        std::cout << utils::CLEAR_SCREEN;
        int choice = select_menu({
            "Calculate Gear Parameters",
            "Load Known Values",
            "Save Current Gear",
            "Settings",
            "Exit"
        });

        switch (choice) {
            case 0: {
                auto params = input_gear_params();
                try {
                    params = gear_calc.calculate(params);
                    int cutter = gear_calc.select_cutter(params.n);
                    std::string div_inst = gear_calc.dividing_head_instructions(params.n);
                    // Involute example
                    double r_base = params.pd / 2.0 * std::cos(params.pa * M_PI / 180.0);
                    auto point = gear_calc.involute_point(r_base, 0.1);  // Sample
                    display_results(params);
                    draw_box("Recommendations", {
                        "Cutter #: " + std::to_string(cutter),
                        div_inst,
                        "Sample Involute Point: x=" + std::to_string(point.first) + ", y=" + std::to_string(point.second)
                    });
                } catch (const std::exception& e) {
                    handle_error(e.what());
                }
                break;
            }
            case 1: {
                auto known = gear_calc.load_known("data/known_values.csv");
                // Display list, select one, etc.
                if (!known.empty()) display_results(known[0]);  // Example
                break;
            }
            case 2: {
                // Assume current params; save
                GearParams dummy;  // Replace with actual
                gear_calc.save(dummy, "data/gears.csv");
                break;
            }
            case 3: show_settings(); break;
            case 4: running = false; break;
        }
        std::cout << "Press enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void Ui::show_settings() {
    // TODO: Implement settings menu (e.g., change colors, but fixed for now)
    draw_box("Settings", {"No customizable settings yet."});
}

GearParams Ui::input_gear_params() {
    GearParams p;
    p.n = 0; p.dp = NAN; p.m = NAN; p.pd = NAN; p.od = NAN; p.rd = NAN;
    p.a = NAN; p.d = NAN; p.wd = NAN; p.cp = NAN; p.pa = NAN; p.cd = NAN; p.backlash = NAN;

    // Prompt for each, allow skip (NAN)
    std::string input;
    std::cout << "Number of teeth (N): "; std::getline(std::cin, input);
    if (!input.empty()) p.n = std::stoi(input);
    
    std::cout << "Diametrical Pitch (DP): "; std::getline(std::cin, input);
    if (!input.empty()) p.dp = utils::safe_stod(input);

    std::cout << "Module (metric)"; std::getline(std::cin, input);
    if (!input.empty()) p.m = utils::safe_stod(input);

    std::cout << "Pitch Diameter"; std::getline(std::cin, input);
    if (!input.empty()) p.pd = utils::safe_stod(input);

    std::cout << "Outside Diameter"; std::getline(std::cin, input);
    if (!input.empty()) p.od = utils::safe_stod(input);

    std::cout << "Root Diameter"; std::getline(std::cin, input);
    if (!input.empty()) p.rd = utils::safe_stod(input);

    std::cout << "Addendum"; std::getline(std::cin, input);
    if (!input.empty()) p.a = utils::safe_stod(input);

    std::cout << "Dedendum"; std::getline(std::cin, input);
    if (!input.empty()) p.d = utils::safe_stod(input);

    std::cout << "Whole Depth"; std::getline(std::cin, input);
    if (!input.empty()) p.wd = utils::safe_stod(input);

    std::cout << "Circular Pitch"; std::getline(std::cin, input);
    if (!input.empty()) p.cp = utils::safe_stod(input);

    std::cout << "Pressure Angle (degrees)"; std::getline(std::cin, input);
    if (!input.empty()) p.pa = utils::safe_stod(input);

    std::cout << "Center Distance (for pair)"; std::getline(std::cin, input);
    if (!input.empty()) p.cd = utils::safe_stod(input);

    std::cout << "Backlash"; std::getline(std::cin, input);
    if (!input.empty()) p.backlash = utils::safe_stod(input);

    return p;
}

void Ui::display_results(const GearParams& params) {
    std::vector<std::string> lines = {
        "N: " + std::to_string(params.n),
        "DP: " + std::to_string(params.dp),
        "M: " + std::to_string(params.m),
        "PD: " + std::to_string(params.pd),
        "OD: " + std::to_string(params.od),
        "RD: " + std::to_string(params.rd),
        "A: " + std::to_string(params.a),
        "D: " + std::to_string(params.d),
        "WD: " + std::to_string(params.wd),
        "CP: " + std::to_string(params.cp),
        "PA: " + std::to_string(params.pa),
        "CD: " + std::to_string(params.cd),
        "Backlash: " + std::to_string(params.backlash)
    };
    draw_box("Gear Parameters", lines);
}

void Ui::handle_error(const std::string& msg) {
    std::cout << utils::COLOR_RED << "Error: " << msg << utils::COLOR_RESET << std::endl;
    LOG(ERROR) << msg;
}

int Ui::select_menu(const std::vector<std::string>& options) {
    int selected = 0; // Stays int because the function returns int
    while (true) {
        std::cout << utils::CLEAR_SCREEN;

        // FIX: Use size_t for the loop counter 'i'
        for (size_t i = 0; i < options.size(); ++i) { 
            // FIX: Cast 'i' to int for comparison with 'selected'
            if (static_cast<int>(i) == selected) std::cout << utils::INVERSE_ON;
            std::cout << "> " << options[i] << utils::INVERSE_OFF << std::endl;
        }
        
        char key = utils::get_key();
        if (key == 'w' || key == 'i') selected = std::max(0, selected - 1);

        // FIX: Cast options.size() for comparison with 'selected'
        else if (key == 's' || key == 'k') selected = std::min(static_cast<int>(options.size()) - 1, selected + 1);

        else if (key == '\n' || key == '\r') return selected;
    }
}

void Ui::run() {
    show_main_screen();
    if (!show_login_register()) return;
    show_main_menu();
}

}  // namespace gearforge
