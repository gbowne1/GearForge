cmake_minimum_required(VERSION 3.10)
project(GearForge VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Find Google Log and Test (assume installed/system)
find_package(Glog REQUIRED)
find_package(GTest REQUIRED)

add_executable(gearforge
    src/main.cpp
    src/gear_calculator.cpp
    src/ui.cpp
    src/user_manager.cpp
    src/utils.cpp
)

target_link_libraries(gearforge glog::glog)

# Tests
add_executable(tests
    tests/main_test.cpp
    tests/ui_test.cpp
    src/gear_calculator.cpp
    src/ui.cpp
    src/utils.cpp
    src/user_manager.cpp
)
target_link_libraries(tests GTest::GTest GTest::Main glog::glog)
enable_testing()
add_test(NAME GearForgeTests COMMAND tests)
