#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "ui.h"
#include "utils.h"

// Stub for user_manager and gear_calc
namespace gearforge {

class UserManagerStub {
public:
    bool login(const std::string&, const std::string&) { return login_result; }
    bool register_user(const std::string&, const std::string&, UserRole = UserRole::User) { return register_result; }
    bool login_result = true;
    bool register_result = true;
};

class GearCalculatorStub {
public:
    GearParams calculate(const GearParams& params) { return params; }
    int select_cutter(int) { return 1; }
    std::string dividing_head_instructions(int) { return "Dividing head instructions"; }
    std::pair<double, double> involute_point(double, double) { return {1.0, 2.0}; }
    std::vector<GearParams> load_known(const std::string&) { return {}; }
    bool save(const GearParams&, const std::string&) { return true; }
};

} // namespace gearforge

// Test fixture for Ui and utils
class UiTest : public ::testing::Test {
protected:
    void SetUp() override {
        original_cout = std::cout.rdbuf();
        std::cout.rdbuf(cout_buffer.rdbuf());
        original_cin = std::cin.rdbuf();
        std::cin.rdbuf(cin_buffer.rdbuf());
        ui.user_manager = UserManagerStub();
        ui.gear_calc = GearCalculatorStub();
    }

    void TearDown() override {
        std::cout.rdbuf(original_cout);
        std::cin.rdbuf(original_cin);
    }

    // Helper to generate expected box output
    std::string generate_box_output(const std::string& title, const std::vector<std::string>& lines) {
        size_t max_len = title.length();
        for (const auto& l : lines) max_len = std::max(max_len, l.length());
        max_len += 4; // Padding

        std::stringstream ss;
        ss << gearforge::utils::COLOR_BLUE << gearforge::utils::BOX_TOP_LEFT;
        for (size_t i = 0; i < max_len - 2; ++i) ss << gearforge::utils::BOX_HORIZONTAL;
        ss << gearforge::utils::BOX_TOP_RIGHT << gearforge::utils::COLOR_RESET << "\n";

        ss << gearforge::utils::COLOR_BLUE << gearforge::utils::BOX_VERTICAL
           << gearforge::utils::COLOR_RESET << " " << title << " ";
        for (size_t i = title.length() + 2; i < max_len - 1; ++i) ss << " ";
        ss << gearforge::utils::COLOR_BLUE << gearforge::utils::BOX_VERTICAL
           << gearforge::utils::COLOR_RESET << "\n";

        for (const auto& line : lines) {
            ss << gearforge::utils::COLOR_BLUE << gearforge::utils::BOX_VERTICAL
               << gearforge::utils::COLOR_RESET << " " << line;
            for (size_t i = line.length() + 1; i < max_len - 1; ++i) ss << " ";
            ss << gearforge::utils::COLOR_BLUE << gearforge::utils::BOX_VERTICAL
               << gearforge::utils::COLOR_RESET << "\n";
        }

        ss << gearforge::utils::COLOR_BLUE << gearforge::utils::BOX_BOTTOM_LEFT;
        for (size_t i = 0; i < max_len - 2; ++i) ss << gearforge::utils::BOX_HORIZONTAL;
        ss << gearforge::utils::BOX_BOTTOM_RIGHT << gearforge::utils::COLOR_RESET << "\n";
        return ss.str();
    }

    std::string get_output() const { return cout_buffer.str(); }
    void clear_buffer() { cout_buffer.str(""); }
    void set_input(const std::string& input) {
        cin_buffer.str(input);
        cin_buffer.seekg(0);
    }

    std::stringstream cout_buffer;
    std::stringstream cin_buffer;
    std::streambuf* original_cout = nullptr;
    std::streambuf* original_cin = nullptr;
    gearforge::Ui ui;
};

// Test draw_box with title only
TEST_F(UiTest, DrawBox_TitleOnly) {
    std::vector<std::string> lines;
    ui.draw_box("Test", lines);
    EXPECT_EQ(get_output(), generate_box_output("Test", lines));
}

// Test draw_box with title and one line
TEST_F(UiTest, DrawBox_TitleAndOneLine) {
    std::vector<std::string> lines = {"Line1"};
    ui.draw_box("Title", lines);
    EXPECT_EQ(get_output(), generate_box_output("Title", lines));
}

// Test draw_box with longer title
TEST_F(UiTest, DrawBox_LongerTitle) {
    std::vector<std::string> lines = {"Short"};
    ui.draw_box("LongTitle", lines);
    EXPECT_EQ(get_output(), generate_box_output("LongTitle", lines));
}

// Test draw_box with empty title
TEST_F(UiTest, DrawBox_EmptyTitle) {
    std::vector<std::string> lines = {"Line1", "Line2"};
    ui.draw_box("", lines);
    EXPECT_EQ(get_output(), generate_box_output("", lines));
}

// Test select_menu navigation
TEST_F(UiTest, SelectMenu_NavigateAndChoose) {
    set_input("s\n"); // Move down once, then Enter
    std::vector<std::string> options = {"Option1", "Option2"};
    int result = ui.select_menu(options);
    EXPECT_EQ(result, 1); // Should select second option
}

// Test select_menu with arrow keys
TEST_F(UiTest, SelectMenu_ArrowKeys) {
    set_input("\033[B\n"); // Down arrow (ESC [ B), then Enter
    std::vector<std::string> options = {"Option1", "Option2"};
    int result = ui.select_menu(options);
    EXPECT_EQ(result, 1); // Should select second option
}

// Test display_results
TEST_F(UiTest, DisplayResults) {
    gearforge::GearParams params;
    params.n = 20;
    params.dp = 10.0;
    params.m = 2.54;
    params.pd = 2.0;
    params.od = 2.2;
    params.rd = 1.8;
    params.a = 0.1;
    params.d = 0.05;
    params.wd = 0.3;
    params.cp = 0.314;
    params.pa = 20.0;
    params.cd = 0.2;
    params.backlash = 0.01;

    std::vector<std::string> lines = {
        "N: 20",
        "DP: 10",
        "M: 2.54",
        "PD: 2",
        "OD: 2.2",
        "RD: 1.8",
        "A: 0.1",
        "D: 0.05",
        "WD: 0.3",
        "CP: 0.314",
        "PA: 20",
        "CD: 0.2",
        "Backlash: 0.01"
    };
    ui.display_results(params);
    EXPECT_EQ(get_output(), generate_box_output("Gear Parameters", lines));
}

// Test show_login_register with successful login
TEST_F(UiTest, ShowLoginRegister_SuccessfulLogin) {
    set_input("0\nuser\npass\n"); // Choose Login, enter username, password
    bool result = ui.show_login_register();
    EXPECT_TRUE(result);
    std::string output = get_output();
    EXPECT_NE(output.find("Username: "), std::string::npos);
    EXPECT_NE(output.find("Password: "), std::string::npos);
}

// Test show_login_register with failed login
TEST_F(UiTest, ShowLoginRegister_FailedLogin) {
    ui.user_manager.login_result = false;
    set_input("0\nuser\npass\n0\nuser\npass\n"); // Fail login, retry, fail again
    bool result = ui.show_login_register();
    EXPECT_FALSE(result); // Should eventually fail or exit
    std::string output = get_output();
    EXPECT_NE(output.find("Error: Login failed."), std::string::npos);
}

// Test input_gear_params
TEST_F(UiTest, InputGearParams) {
    set_input("20\n10\n\n"); // Enter N, DP, skip rest
    gearforge::GearParams params = ui.input_gear_params();
    EXPECT_EQ(params.n, 20);
    EXPECT_DOUBLE_EQ(params.dp, 10.0);
    EXPECT_TRUE(std::isnan(params.m));
}

// Test handle_error
TEST_F(UiTest, HandleError) {
    ui.handle_error("Test error");
    std::string output = get_output();
    std::string expected = gearforge::utils::COLOR_RED + "Error: Test error" +
                          gearforge::utils::COLOR_RESET + "\n";
    EXPECT_EQ(output, expected);
}

// Utility tests
TEST(UtilityTest, TrimString) {
    EXPECT_EQ(gearforge::utils::trim("  hello  "), "hello");
    EXPECT_EQ(gearforge::utils::trim("\tworld\t"), "world");
    EXPECT_EQ(gearforge::utils::trim("   "), "");
}

TEST(UtilityTest, SafeStoD) {
    EXPECT_DOUBLE_EQ(gearforge::utils::safe_stod("123.45"), 123.45);
    EXPECT_THROW(gearforge::utils::safe_stod("invalid"), std::runtime_error);
    EXPECT_THROW(gearforge::utils::safe_stod("1e308"), std::runtime_error);
}

TEST(UtilityTest, ReadCsv) {
    std::string filename = "test.csv";
    std::vector<std::vector<std::string>> data = {{"a", "b"}, {"1", "2"}};
    ASSERT_TRUE(gearforge::utils::write_csv(filename, data));
    
    auto result = gearforge::utils::read_csv(filename);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], std::vector<std::string>({"a", "b"}));
    EXPECT_EQ(result[1], std::vector<std::string>({"1", "2"}));
    
    ASSERT_TRUE(std::filesystem::remove(filename));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
