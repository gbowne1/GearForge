#!/bin/bash

# GearForge direct compilation script
# Supports GCC or Clang on Unix-like systems (FreeBSD, Arch, Debian-based, Gentoo, RHEL, Alpine, NixOS)

set -e

# Compiler (default: g++; override with CC=clang++)
CC=${CC:-g++}

# Flags
CXXFLAGS="-std=c++17 -Wall -Werror -Wpedantic -pedantic -pedantic-errors -Iinclude"
LDFLAGS="-lglog"

# Sources
SOURCES="src/main.cpp src/gear_calculator.cpp src/ui.cpp src/user_manager.cpp src/utils.cpp"
TEST_SOURCES="tests/main_test.cpp src/gear_calculator.cpp src/utils.cpp src/user_manager.cpp"

# Output
mkdir -p build
OUT=build/gearforge
TEST_OUT=build/tests

echo "Compiling GearForge with $CC..."

# Compile main program
$CC $CXXFLAGS $SOURCES -o $OUT $LDFLAGS

echo "Compiling tests..."
$CC $CXXFLAGS $TEST_SOURCES -o $TEST_OUT -lgtest -lgtest_main -pthread $LDFLAGS

echo "Build complete. Run:"
echo "  $OUT  # Main program"
echo "  $TEST_OUT  # Run tests"
