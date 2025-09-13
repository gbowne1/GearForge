#pragma once

#include "gear_calculator.h"
#include "user_manager.h"
#include "utils.h"

namespace gearforge {

class Ui {
private:
    UserManager& user_manager;
    GearCalculator gear_calc;
    bool running = true;

    void draw_box(const std::string& title, const std::vector<std::string>& lines);
    void show_main_screen();
    bool show_login_register();
    void show_main_menu();
    void show_settings();
    GearParams input_gear_params();
    void display_results(const GearParams& params);
    void handle_error(const std::string& msg);
    int select_menu(const std::vector<std::string>& options);

public:
    Ui(UserManager& um) : user_manager(um) {}
    void run();
};

}  // namespace gearforge
