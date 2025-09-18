# Testing Guide for GearForge

This guide explains how to write and run unit tests for the GearForge project using Google Test. 
It covers the project's test structure, how to create new test files, and how to execute tests with CMake.

## Prerequisites

- **Google Test**: Ensure Google Test is installed
- **Google Log**: Ensure Google Log is installed
- **CMake**: Version 3.10 or higher.
- **C++ Compiler**: Supporting C++17 (e.g., `g++`).
- **Project Setup**: Clone the GearForge repository and ensure the `CMakeLists.txt` is configured as described below.

## Documentation for Google

https://google.github.io/googletest/
https://google.github.io/glog/stable/#

## Project Structure

- **Source Files**: Located in `src/` (e.g., `main.cpp`, `ui.cpp`, `gear_calculator.cpp`, `user_manager.cpp`, `utils.cpp`).
- **Headers**: Located in `include/` (e.g., `ui.h`, `gear_calculator.h`, `user_manager.h`, `utils.h`).
- **Tests**: Located in `tests/` with the naming convention `thing_test.cpp` (e.g., `main_test.cpp`, `ui_test.cpp`).

The `CMakeLists.txt` in the project root builds two executables:
- `gearforge`: The main application.
- `tests`: The test executable, including all `*_test.cpp` files in `tests/`.

## Writing Tests

### Test File Naming and Location
- Create test files in the `tests/` directory.
- Name test files using the pattern `thing_test.cpp`, where `thing` corresponds to the component being tested 
  (e.g., `ui_test.cpp` for the `Ui` class, `gear_calculator_test.cpp` for `GearCalculator`).
- Each test file should include the relevant header from `include/` and use Google Test macros.

### Test Structure
- Use the `TEST` macro for standalone tests or `TEST_F` for tests using a fixture.
- Include necessary headers (e.g., `<gtest/gtest.h>`, project headers like `"ui.h"`).
- Use stubs or mocks for dependencies to isolate the component being tested.


    EXPECT_NE(output.find("Test"), std::string::npos);
}
