#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "ui.h"
#include "utils.h"

// Stub for user_manager and gear_calc to avoid undefined references
namespace gearforge {

class UserManagerStub {
public:
    bool login(const std::string&, const std::string&) { return true; }
    bool register_user(const std::string&, const std::string&) { return true; }
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
        // Redirect std::cout
        original_cout = std::cout.rdbuf();
        std::cout.rdbuf(cout_buffer.rdbuf());

        // Redirect std::cin
        original_cin = std::cin.rdbuf();
        std::cin.rdbuf(cin_buffer.rdbuf());

        // Initialize Ui with stubs
        ui.user_manager = UserManagerStub();
        ui.gear_calc = GearCalculatorStub();
    }

    void TearDown() override {
        // Restore std::cout and std::cin
        std::cout.rdbuf(original_cout);
        std::cin.rdbuf(original_cin);
    }

    // Helper to get captured output
    std::string get_output() const { return cout_buffer.str(); }

    // Helper to clear output buffer
    void clear_buffer() { cout_buffer.str(""); }

    // Helper to set input
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
    ui.draw_box("Test", {});
    std::string output = get_output();

    // Expected output:
    // ┌────────┐
    // │ Test   │
    // └────────┘
    std::string expected =
        gearforge::utils::COLOR_BLUE + "┌" +
        "────────" +
        "┐" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + " Test " +
        "   " +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "└" +
        "────────" +
        "┘" + gearforge::utils::COLOR_RESET + "\n";

    EXPECT_EQ(output, expected);
}

// Test draw_box with title and one line
TEST_F(UiTest, DrawBox_TitleAndOneLine) {
    ui.draw_box("Title", {"Line1"});
    std::string output = get_output();

    // Expected output:
    // ┌────────┐
    // │ Title  │
    // │ Line1  │
    // └────────┘
    std::string expected =
        gearforge::utils::COLOR_BLUE + "┌" +
        "────────" +
        "┐" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + " Title " +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + " Line1 " +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "└" +
        "────────" +
        "┘" + gearforge::utils::COLOR_RESET + "\n";

    EXPECT_EQ(output, expected);
}

// Test draw_box with longer title
TEST_F(UiTest, DrawBox_LongerTitle) {
    ui.draw_box("LongTitle", {"Short"});
    std::string output = get_output();

    // Expected output:
    // ┌───────────┐
    // │ LongTitle │
    // │ Short     │
    // └───────────┘
    std::string expected =
        gearforge::utils::COLOR_BLUE + "┌" +
        "───────────" +
        "┐" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + " LongTitle " +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + " Short " +
        "    " +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "└" +
        "───────────" +
        "┘" + gearforge::utils::COLOR_RESET + "\n";

    EXPECT_EQ(output, expected);
}

// Test select_menu with simulated input
TEST_F(UiTest, SelectMenu_ChooseFirstOption) {
    set_input("\n"); // Simulate pressing Enter
    std::vector<std::string> options = {"Option1", "Option2"};
    int result = ui.select_menu(options);
    EXPECT_EQ(result, 0); // Should select first option
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

    ui.display_results(params);
    std::string output = get_output();

    // Expected output (simplified; actual will depend on formatting)
    std::string expected =
        gearforge::utils::COLOR_BLUE + "┌" +
        "───────────────────" +
        "┐" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + " Gear Parameters " +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + " N: 20 " +
        "           " +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + "\n" +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + " DP: 10 " +
        "          " +
        gearforge::utils::COLOR_BLUE + "│" + gearforge::utils::COLOR_RESET + "\n" +
        // ... (similar lines for other params) ...
        gearforge::utils::COLOR_BLUE + "└" +
        "───────────────────" +
        "┘" + gearforge::utils::COLOR_RESET + "\n";

    EXPECT_EQ(output, expected);
}

// Test utils::trim
TEST(UtilityTest, TrimString) {
    EXPECT_EQ(gearforge::utils::trim("  hello  "), "hello");
    EXPECT_EQ(gearforge::utils::trim("\tworld\t"), "world");
    EXPECT_EQ(gearforge::utils::trim("   "), "");
}

// Test utils::safe_stod
TEST(UtilityTest, SafeStoD) {
    EXPECT_DOUBLE_EQ(gearforge::utils::safe_stod("123.45"), 123.45);
    EXPECT_THROW(gearforge::utils::safe_stod("invalid"), std::runtime_error);
    EXPECT_THROW(gearforge::utils::safe_stod("1e308"), std::runtime_error);
}

// Test utils::read_csv (requires a temporary file)
TEST(UtilityTest, ReadCsv) {
    std::string filename = "test.csv";
    std::vector<std::vector<std::string>> data = {{"a", "b"}, {"1", "2"}};
    gearforge::utils::write_csv(filename, data);
    
    auto result = gearforge::utils::read_csv(filename);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], std::vector<std::string>({"a", "b"}));
    EXPECT_EQ(result[1], std::vector<std::string>({"1", "2"}));
    
    std::filesystem::remove(filename); // Clean up
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
