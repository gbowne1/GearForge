#include <gtest/gtest.h>
#include "gear_calculator.h"
#include "utils.h"

TEST(GearCalculatorTest, BasicCalc) {
    gearforge::GearParams input;
    input.n = 20;
    input.dp = 10.0;
    input.pa = 20.0;
    gearforge::GearCalculator calc;
    auto result = calc.calculate(input);
    EXPECT_DOUBLE_EQ(result.pd, 2.0);
    EXPECT_DOUBLE_EQ(result.a, 0.1);
    // ... more asserts
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
