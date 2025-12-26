CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lglog -lgflags
TEST_LDFLAGS = -lgtest -lgtest_main -pthread

SOURCES = src/main.cpp src/gear_calculator.cpp src/ui.cpp src/user_manager.cpp src/utils.cpp
TEST_SOURCES = tests/main_test.cpp src/gear_calculator.cpp src/utils.cpp src/user_manager.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

OUT = build/gearforge
TEST_OUT = build/tests

all: $(OUT) $(TEST_OUT)

$(OUT): $(OBJECTS)
	@mkdir -p build
	$(CXX) $(OBJECTS) -o $(OUT) $(LDFLAGS)

$(TEST_OUT): $(TEST_OBJECTS)
	@mkdir -p build
	$(CXX) $(TEST_OBJECTS) -o $(TEST_OUT) $(TEST_LDFLAGS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build *.o src/*.o tests/*.o

test: $(TEST_OUT)
	./$(TEST_OUT)

.PHONY: all clean test
