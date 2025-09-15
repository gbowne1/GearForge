# GearForge User Guide

## Overview

GearForge is a terminal-based tool designed for machine shops to calculate spur gear parameters for milling on horizontal milling machines. It provides calculations for gear dimensions, involute cutter selection, and dividing head instructions (40:1 ratio). The interface uses ANSI colors, box-drawing characters, and WASD/arrow key navigation.

## Getting Started

### Prerequisites

64-bit Intel x86 system
Terminal emulator supporting ANSI escape codes and Unicode
GearForge binary (see Installation)

## Installation

### Clone the repository: 

`git clone https://github.com/gbowne1/gearforge.git`

`cd gearforge`

### Install dependencies

While you can compile this with other build systems and compilers, these dependencies are for CMake.
This requires CMake 3.10 or newer.

Fedora: install cmake, glog, glog-devel, gtest

Arch: install gtest, google-glog, cmake

Redhat:

Debian: install cmake, libgoogle-glog-dev, libgtest-dev

Ubuntu: install cmake, libgoogle-glog-dev, libgtest-dev

FreeBSD: install googletest, glog, cmake, 

Alpine: install cmake, glog, gtest

Gentoo: install dev-cpp/glog, dev-cpp/gtest, dev-build/cmake

NixOS: install cmake, 

Solaris:

Windows: Use winget. Kitware.CMake, 

MacOSX: 

To build GearForge as gearforge

`mkdir build && cd build`
`cmake ..`
`make`

To run GearForge as gearforge

`./gearforge`

### Command-Line Options

Flag | Description
--help | Show help message
--version | Show version (0.0.1)
--load=<file.csv> | Load gear parameters from CSV

## Using GearForge

### Startup

On launch, GearForge displays a splash screen with the program name, version (0.0.1), and copyright (© 2025). After 3 seconds, it clears to the login/register screen.

### Login/Register

Register: Enter a username and password to create an account. Passwords are hashed (SHA256) and stored in data/users.csv.
Login: Enter existing credentials. Use WASD or arrow keys to select options, Enter to confirm.
Exit: Exit the program.

### Main Menu

Navigate with WASD, IJKL, or arrow keys (highlight with inverse text). Options:

Calculate Gear Parameters: Input gear data.
Load Known Values: Load from data/known_values.csv.
Save Current Gear: Save to data/gears.csv.
Settings: (Limited; future expansion).
Exit: Quit.

Gear Calculations

Select "Calculate Gear Parameters."
Enter values (e.g., Number of Teeth, Diametrical Pitch, or Module). Leave blank to skip (defaults applied where possible).
Supported Variables:
N: Number of teeth (integer)
DP: Diametrical Pitch (e.g., 10.0)
M: Module (metric, e.g., 2.5)
PA: Pressure Angle (degrees, e.g., 20.0)
CD: Center Distance (for gear pairs)
Others calculated (PD, OD, RD, A, D, WD, CP, Backlash)

View results in a box-drawn table, including:
Calculated parameters
Recommended involute cutter number (1–8)
Dividing head instructions (e.g., "3 full turns + 0.5 fractional")
Sample involute curve point (x, y)

## Navigation

Use W (up), S (down), A (left), D (right), or arrow keys.
Enter confirms selections.
Errors/warnings appear in red; press Enter to continue.

## Data Management

Save: Gear parameters are saved to data/gears.csv.
Load: Load known gear configs from data/known_values.csv.
Progress bars (e.g., [====> ] 50%) show during file operations.

## Colors

Element| Color
Text | White
Menus/Boxes | Blue
Errors | Red
Progress | Green
Highlights | Yellow (inverse)

## Troubleshooting

Error: Invalid input: Ensure numbers are valid (e.g., positive integers for N).
No data folder: Created automatically on first run.
Terminal issues: Ensure your terminal supports ANSI/Unicode (e.g., xterm, GNOME Terminal).

For developer details, see developer_guide.md.
