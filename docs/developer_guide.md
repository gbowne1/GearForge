# GearForge Developer Guide

## Overview

GearForge is a C++11 terminal application for spur gear machining calculations. This guide covers building, testing, extending, and contributing to the project.

## Project Structure

```shell
GearForge/
├── CMakeLists.txt         # CMake build configuration
├── LICENSE               # MIT License
├── README.md             # GitHub landing page
├── data/                 # CSV files (users.csv, gears.csv, known_values.csv)
├── docs/                 # Documentation
│   ├── user_guide.md
│   ├── developer_guide.md
├── include/              # Header files
│   ├── gear_calculator.h
│   ├── ui.h
│   ├── user_manager.h
│   ├── utils.h
├── src/                  # Source files
│   ├── gear_calculator.cpp
│   ├── main.cpp
│   ├── ui.cpp
│   ├── user_manager.cpp
│   ├── utils.cpp
├── tests/                # Unit tests
│   └── main_test.cpp
```

## Dependencies

CMake: 3.10+
Google Log: Logging library
Google Test: Testing framework
Standard C++11 libraries: No other 3rd-party dependencies

Install on distros with apt/Aptitude or synaptic:
sudo apt-get install cmake libglog-dev googletest

## Building

Clone the repository:  `git clone https://github.com/gbowne1/gearforge.git`
cd gearforge

### Build:

mkdir build && cd build
cmake ..
make

Run:./gearforge

Run tests:./tests

## Code Organization

Namespaces: gearforge for all project code, gearforge::utils for utilities.

Classes (PascalCase):

GearCalculator: Handles gear math, cutter selection, dividing head instructions.

UserManager: Manages user authentication (SHA256 hashing).

Ui: Terminal interface with menus, navigation, and ANSI rendering.

Files (lowercase):

Headers in include/ with #pragma once.

Sources in src/.

Includes: Ordered as system (<>), Google libs, project headers.

Data: CSV files in data/ for users, gear configs, known values.

Standards: C++11, 15-digit double precision, no external libs beyond glog/gtest.

## Key Components

Gear Calculations

GearParams struct: Holds N, DP, M, PD, OD, RD, A, D, WD, CP, PA, CD, Backlash.

Formulas (in GearCalculator::calculate):
PD = N / DP
A = 1 / DP
D = 1.157 / DP
OD = PD + 2A
RD = PD - 2D
CP = π / DP
Backlash = 0.003 * PD (default)
M = 25.4 / DP (if metric)


Involute Curve: Parametric equations in involute_point:x = r_base * (cos(θ) + θ * sin(θ))
y = r_base * (sin(θ) - θ * cos(θ))

where r_base = PD/2 * cos(PA).

## UI

- ANSI Escapes: Colors (Black, White, Blue, Gray, Yellow, Red, Green), clearing (\033[2J), inverse text (\033[7m).

- Box Drawing: Unicode chars (┌─┐│).

- Navigation: WASD/IJKL/arrows via utils::get_key() with system("stty raw").

- Progress Bars: Text-based ([====> ] 50%).

## Security

UserManager: Stores users in data/users.csv with SHA256-hashed passwords.
Permissions: Simple UserRole enum (User, Admin).

## Utilities

utils.h/cpp: SHA256, CSV handling, progress bars, string trimming, key input.

## Extending GearForge

### Adding Features

New Calculations: Extend GearCalculator with new methods, update GearParams.

UI Enhancements: Add menu options in Ui::show_main_menu.

Data Formats: Modify utils::read_csv/write_csv for new formats.

Example: 

Adding Metric-Only Mode

- Add flag to main.cpp:if (arg == "--metric") { /* Set mode */ }

- Update GearCalculator::calculate to prioritize Module (M).

## Testing

Tests in tests/main_test.cpp use Google Test.

Example test:

```cpp
TEST(GearCalculatorTest, BasicCalc) {
    GearParams input;
    input.n = 20;
    input.dp = 10.0;
    GearCalculator calc;
    auto result = calc.calculate(input);
    EXPECT_DOUBLE_EQ(result.pd, 2.0);
}
```

Run: ./tests

## Contributing

- Fork or clone the repo.

- Create a feature branch: git checkout -b feature/yourfeature.

- Commit changes: git commit -m "Add your feature".

- Push: git push origin feature/yourfeature.

Open a pull request on GitHub.

## Coding Standards

C++11: Use std::shared_ptr, std::vector, std::map, etc.
Naming: PascalCase for classes, lowercase for files/functions/vars.
Returns: Use int, bool, etc., not void unless appropriate.
Includes: System first, then glog/gtest, then project headers.
Precision: Assume 15-digit double precision.

## License

MIT License (see LICENSE).
