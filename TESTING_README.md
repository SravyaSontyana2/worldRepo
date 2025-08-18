# Testing Guide for Adaptive Cruise Control System

This document provides comprehensive information about running unit tests for the Adaptive Cruise Control (ACC) system.

## Overview

The project includes two testing approaches:

1. **Simple Tests** (`simple_test.cpp`) - No external dependencies, easy to run
2. **Google Test Framework** (`test_AdaptiveCruiseControl.cpp`) - Professional testing framework with advanced features

## Quick Start

### Option 1: Simple Tests (Recommended for beginners)

```bash
# Build and run simple tests
make test

# Or build everything first, then test
make build-all
make test
```

### Option 2: Google Test Framework

```bash
# Build and run Google Test (requires Google Test installation)
make test-gtest
```

## Test Coverage

The test suite covers the following areas:

### 1. Constructor Tests
- Default constructor with zero values
- Custom constructor with specific parameters
- Log file name initialization

### 2. Input Validation Tests
- `updateAheadVehicleSpeed()` with valid and invalid inputs
- `updateDistance()` with valid and invalid inputs
- Negative value handling

### 3. Core Algorithm Tests
- `calculateSafeDistance()` with various speeds
- 2-second rule calculation verification
- Edge cases (zero speed, very high speed)

### 4. Speed Adjustment Tests
- **Too Close Scenario**: Distance < safe distance
  - Speed reduction to match ahead vehicle
  - Emergency braking behavior
- **Safe Distance Scenario**: Distance between safe and 1.5×safe
  - Speed maintenance
- **Good Distance Scenario**: Distance > 1.5×safe
  - Speed increase when appropriate
- **Speed Limit Enforcement**: Maximum 120 km/h

### 5. Edge Case Tests
- Very high speeds (200+ km/h)
- Very small distances (0.1 m)
- Zero speed scenarios
- Boundary conditions

### 6. File I/O Tests
- Log file creation and writing
- File content verification
- Error handling for invalid paths

### 7. Integration Tests
- Multiple consecutive speed adjustments
- Dynamic scenario updates
- State consistency across operations

## Test Scenarios Explained

### Safe Distance Calculation
The system uses the **2-second rule**:
- Formula: `safe_distance = speed × (5/9)`
- At 60 km/h: safe distance = 33.33 meters
- At 120 km/h: safe distance = 66.67 meters

### Speed Adjustment Logic
1. **Too Close** (< safe distance):
   - If ahead vehicle is slower: match its speed
   - Otherwise: reduce speed by 5 km/h

2. **Safe Range** (safe distance to 1.5×safe):
   - Maintain current speed

3. **Good Distance** (> 1.5×safe):
   - If ahead vehicle is faster: increase speed by 2 km/h
   - Respect 120 km/h speed limit

## Building and Running Tests

### Prerequisites

#### For Simple Tests
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- No external libraries required

#### For Google Test
- C++17 compatible compiler
- Google Test library installed

### Installation of Google Test

#### Windows (vcpkg)
```bash
vcpkg install gtest
```

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install libgtest-dev
```

#### macOS (Homebrew)
```bash
brew install googletest
```

#### Manual Installation
```bash
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
make
sudo make install
```

### Build Commands

```bash
# Build main project only
make

# Build main project and simple tests
make build-all

# Build and run simple tests
make test

# Build and run Google Test
make test-gtest

# Clean all build files
make clean

# Show available targets
make help
```

### CMake Alternative

If you prefer CMake:

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build .

# Run tests (if Google Test is found)
ctest
```

## Test Output Examples

### Simple Test Output
```
=== Simple Unit Tests for Adaptive Cruise Control ===
Running tests...

Testing constructor... PASS
Testing updateAheadVehicleSpeed... PASS
Testing updateDistance... PASS
Testing calculateSafeDistance... PASS
Testing adjustSpeed... PASS
Testing speed limit enforcement... PASS
Testing log file functionality... PASS
Testing edge cases... PASS

=== Test Results ===
Passed: 8/8
All tests PASSED! 🎉
```

### Google Test Output
```
[==========] Running 25 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from AdaptiveCruiseControlTest
[ RUN      ] AdaptiveCruiseControlTest.ConstructorDefaultValues
[       OK ] AdaptiveCruiseControlTest.ConstructorDefaultValues (0 ms)
...
[----------] 25 tests from AdaptiveCruiseControlTest (0 ms total)

[----------] Global test environment tear-down.
[==========] 25 tests from 1 test case ran. (0 ms total)
[  PASSED  ] 25 tests.
```

## Debugging Failed Tests

### Common Issues

1. **Compilation Errors**
   - Check C++ standard compatibility (C++17 required)
   - Verify all header files are present

2. **Test Failures**
   - Review test output for specific failure details
   - Check expected vs. actual values
   - Verify test assumptions match implementation

3. **File I/O Issues**
   - Ensure write permissions in test directory
   - Check for file path conflicts

### Debug Mode

Build with debug flags:

```bash
# Add debug information
CXXFLAGS="-std=c++17 -Wall -Wextra -g -O0" make test

# Or modify Makefile temporarily
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0
```

## Adding New Tests

### For Simple Tests

1. Add new test function in `simple_test.cpp`:
```cpp
bool testNewFeature() {
    std::cout << "Testing new feature... ";
    
    // Test implementation
    AdaptiveCruiseControl acc;
    // ... test logic ...
    
    std::cout << "PASS" << std::endl;
    return true;
}
```

2. Add to main function:
```cpp
if (testNewFeature()) passed++; total++;
```

### For Google Test

1. Add new test in `test_AdaptiveCruiseControl.cpp`:
```cpp
TEST_F(AdaptiveCruiseControlTest, NewFeature) {
    AdaptiveCruiseControl acc;
    // ... test logic ...
    EXPECT_EQ(expected, actual);
}
```

## Continuous Integration

### GitHub Actions Example

```yaml
name: Tests
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Install dependencies
      run: sudo apt-get install libgtest-dev
    - name: Build and test
      run: |
        make build-all
        make test
```

## Performance Testing

For performance-critical scenarios, add timing tests:

```cpp
#include <chrono>

auto start = std::chrono::high_resolution_clock::now();
// ... operation ...
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
std::cout << "Operation took: " << duration.count() << " microseconds" << std::endl;
```

## Coverage Analysis

### Using gcov (GCC)

```bash
# Build with coverage flags
CXXFLAGS="-std=c++17 -Wall -Wextra -fprofile-arcs -ftest-coverage" make test

# Run tests
./simple_tests

# Generate coverage report
gcov AdaptiveCruiseControl.cpp
```

## Best Practices

1. **Test Naming**: Use descriptive test names that explain the scenario
2. **Isolation**: Each test should be independent and not affect others
3. **Cleanup**: Always clean up resources (files, memory) in test teardown
4. **Edge Cases**: Test boundary conditions and invalid inputs
5. **Documentation**: Document complex test scenarios and expected behaviors

## Troubleshooting

### "Google Test not found" Error
- Install Google Test library (see installation section)
- Verify library path in CMake configuration
- Check pkg-config or find_package configuration

### Test Compilation Failures
- Ensure C++17 standard is enabled
- Check for missing includes or dependencies
- Verify compiler compatibility

### Test Runtime Failures
- Review test output for specific error messages
- Check file permissions and paths
- Verify test environment setup

## Support

For issues with:
- **Tests**: Check test output and error messages
- **Build System**: Verify Makefile or CMake configuration
- **Dependencies**: Ensure required libraries are installed
- **Implementation**: Review source code and test assumptions

## Contributing

When adding new features:
1. Write tests first (TDD approach)
2. Ensure all tests pass
3. Add tests for edge cases
4. Update this documentation
5. Verify test coverage

---

**Note**: The simple test framework is designed to work without external dependencies and provides comprehensive coverage of the ACC system functionality. Use Google Test for more advanced testing features and CI/CD integration.
