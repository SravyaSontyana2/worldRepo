# Makefile for Adaptive Cruise Control Project
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = acc_system
SOURCES = main.cpp AdaptiveCruiseControl.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = AdaptiveCruiseControl.h

# Test targets
TEST_TARGET = acc_tests
SIMPLE_TEST_TARGET = simple_tests
TEST_SOURCES = test_AdaptiveCruiseControl.cpp AdaptiveCruiseControl.cpp
SIMPLE_TEST_SOURCES = simple_test.cpp AdaptiveCruiseControl.cpp

# Default target
all: $(TARGET)

# Link the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build Google Test executable (requires Google Test)
$(TEST_TARGET): test_AdaptiveCruiseControl.o AdaptiveCruiseControl.o
	$(CXX) test_AdaptiveCruiseControl.o AdaptiveCruiseControl.o -lgtest -lgtest_main -lpthread -o $(TEST_TARGET)

# Build simple test executable (no external dependencies)
$(SIMPLE_TEST_TARGET): simple_test.o AdaptiveCruiseControl.o
	$(CXX) simple_test.o AdaptiveCruiseControl.o -o $(SIMPLE_TEST_TARGET)

# Compile test files
test_AdaptiveCruiseControl.o: test_AdaptiveCruiseControl.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c test_AdaptiveCruiseControl.cpp -o test_AdaptiveCruiseControl.o

simple_test.o: simple_test.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c simple_test.cpp -o simple_test.o

# Clean build files
clean:
	del /Q *.o $(TARGET).exe $(TEST_TARGET).exe $(SIMPLE_TEST_TARGET).exe 2>nul || rm -f *.o $(TARGET) $(TEST_TARGET) $(SIMPLE_TEST_TARGET)

# Run the main program
run: $(TARGET)
	./$(TARGET)

# Run simple tests
test: $(SIMPLE_TEST_TARGET)
	./$(SIMPLE_TEST_TARGET)

# Run Google Test (if available)
test-gtest: $(TEST_TARGET)
	./$(TEST_TARGET)

# Build all targets
build-all: $(TARGET) $(SIMPLE_TEST_TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all        - Build the main project (default)"
	@echo "  build-all  - Build main project and all tests"
	@echo "  test       - Build and run simple tests (no external dependencies)"
	@echo "  test-gtest - Build and run Google Test (requires Google Test)"
	@echo "  clean      - Remove build files"
	@echo "  run        - Build and run the main program"
	@echo "  help       - Show this help message"

.PHONY: all clean run test test-gtest build-all help
