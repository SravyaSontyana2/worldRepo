cmake_minimum_required(VERSION 3.14)
project(AdaptiveCruiseControl)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Main executable
add_executable(acc_system 
    main.cpp 
    AdaptiveCruiseControl.cpp
)

# Include directories
target_include_directories(acc_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Unit tests (optional - only if Google Test is available)
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    # Try to find Google Test
    find_package(GTest QUIET)
    
    if(GTest_FOUND)
        message(STATUS "Google Test found, building tests")
        
        # Test executable
        add_executable(acc_tests 
            test_AdaptiveCruiseControl.cpp 
            AdaptiveCruiseControl.cpp
        )
        
        # Link with Google Test
        target_link_libraries(acc_tests GTest::GTest GTest::Main)
        
        # Include directories
        target_include_directories(acc_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        
        # Enable CTest
        enable_testing()
        add_test(NAME ACC_Tests COMMAND acc_tests)
        
    else()
        message(WARNING "Google Test not found. Install Google Test to build tests.")
        message(STATUS "On Windows with vcpkg: vcpkg install gtest")
        message(STATUS "On Ubuntu/Debian: sudo apt-get install libgtest-dev")
        message(STATUS "On macOS with Homebrew: brew install googletest")
    endif()
endif()

# Install targets
install(TARGETS acc_system DESTINATION bin)

# Print configuration summary
message(STATUS "Configuration Summary:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Tests: ${BUILD_TESTS}")
if(BUILD_TESTS AND GTest_FOUND)
    message(STATUS "  Google Test: Found")
else()
    message(STATUS "  Google Test: Not found")
endif()
