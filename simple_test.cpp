#include "AdaptiveCruiseControl.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <cstdio>

// Simple assertion macro
#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) { \
        std::cout << "FAIL: Expected " << (expected) << ", got " << (actual) << std::endl; \
        return false; \
    }

#define ASSERT_DOUBLE_EQUAL(expected, actual, tolerance) \
    if (std::abs((expected) - (actual)) > (tolerance)) { \
        std::cout << "FAIL: Expected " << (expected) << ", got " << (actual) << std::endl; \
        return false; \
    }

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        std::cout << "FAIL: Expected true, got false" << std::endl; \
        return false; \
    }

// Test functions
bool testConstructor() {
    std::cout << "Testing constructor... ";
    
    AdaptiveCruiseControl acc;
    ASSERT_EQUAL(0.0, acc.getEgoSpeed());
    ASSERT_EQUAL(0.0, acc.getAheadVehicleSpeed());
    ASSERT_EQUAL(0.0, acc.getDistance());
    
    AdaptiveCruiseControl acc2(80.0, 75.0, 50.0, "test.txt");
    ASSERT_EQUAL(80.0, acc2.getEgoSpeed());
    ASSERT_EQUAL(75.0, acc2.getAheadVehicleSpeed());
    ASSERT_EQUAL(50.0, acc2.getDistance());
    
    std::cout << "PASS" << std::endl;
    return true;
}

bool testUpdateAheadVehicleSpeed() {
    std::cout << "Testing updateAheadVehicleSpeed... ";
    
    AdaptiveCruiseControl acc;
    acc.updateAheadVehicleSpeed(60.0);
    ASSERT_EQUAL(60.0, acc.getAheadVehicleSpeed());
    
    // Test negative speed (should not change)
    acc.updateAheadVehicleSpeed(-10.0);
    ASSERT_EQUAL(60.0, acc.getAheadVehicleSpeed());
    
    std::cout << "PASS" << std::endl;
    return true;
}

bool testUpdateDistance() {
    std::cout << "Testing updateDistance... ";
    
    AdaptiveCruiseControl acc;
    acc.updateDistance(25.0);
    ASSERT_EQUAL(25.0, acc.getDistance());
    
    // Test negative distance (should not change)
    acc.updateDistance(-5.0);
    ASSERT_EQUAL(25.0, acc.getDistance());
    
    std::cout << "PASS" << std::endl;
    return true;
}

bool testCalculateSafeDistance() {
    std::cout << "Testing calculateSafeDistance... ";
    
    AdaptiveCruiseControl acc1(0.0, 0.0, 0.0);
    ASSERT_EQUAL(0.0, acc1.calculateSafeDistance());
    
    AdaptiveCruiseControl acc2(60.0, 0.0, 0.0);
    double expected = 60.0 * (5.0 / 9.0);
    ASSERT_DOUBLE_EQUAL(expected, acc2.calculateSafeDistance(), 0.001);
    
    AdaptiveCruiseControl acc3(120.0, 0.0, 0.0);
    expected = 120.0 * (5.0 / 9.0);
    ASSERT_DOUBLE_EQUAL(expected, acc3.calculateSafeDistance(), 0.001);
    
    std::cout << "PASS" << std::endl;
    return true;
}

bool testAdjustSpeed() {
    std::cout << "Testing adjustSpeed... ";
    
    // Test too close scenario
    AdaptiveCruiseControl acc1(80.0, 70.0, 20.0);
    double safeDistance = acc1.calculateSafeDistance();
    ASSERT_TRUE(acc1.getDistance() < safeDistance);
    
    acc1.adjustSpeed();
    ASSERT_EQUAL(70.0, acc1.getEgoSpeed());
    
    // Test safe distance scenario
    AdaptiveCruiseControl acc2(60.0, 65.0, 50.0);
    double initialSpeed = acc2.getEgoSpeed();
    safeDistance = acc2.calculateSafeDistance();
    ASSERT_TRUE(acc2.getDistance() > safeDistance);
    ASSERT_TRUE(acc2.getDistance() < safeDistance * 1.5);
    
    acc2.adjustSpeed();
    ASSERT_EQUAL(initialSpeed, acc2.getEgoSpeed());
    
    // Test good distance scenario
    AdaptiveCruiseControl acc3(70.0, 80.0, 60.0);
    initialSpeed = acc3.getEgoSpeed();
    safeDistance = acc3.calculateSafeDistance();
    ASSERT_TRUE(acc3.getDistance() > safeDistance * 1.5);
    ASSERT_TRUE(acc3.getAheadVehicleSpeed() > acc3.getEgoSpeed());
    
    acc3.adjustSpeed();
    ASSERT_EQUAL(initialSpeed + 2.0, acc3.getEgoSpeed());
    
    std::cout << "PASS" << std::endl;
    return true;
}

bool testSpeedLimit() {
    std::cout << "Testing speed limit enforcement... ";
    
    // Choose a distance that is comfortably > 1.5 * safe distance
    // For 118 km/h, safe ≈ 65.56, 1.5×safe ≈ 98.33, so 120 m works
    AdaptiveCruiseControl acc(118.0, 125.0, 120.0);
    double safeDistance = acc.calculateSafeDistance();
    ASSERT_TRUE(acc.getDistance() > safeDistance * 1.5);
    ASSERT_TRUE(acc.getAheadVehicleSpeed() > acc.getEgoSpeed());
    
    acc.adjustSpeed();
    ASSERT_EQUAL(120.0, acc.getEgoSpeed());
    
    std::cout << "PASS" << std::endl;
    return true;
}

bool testLogFile() {
    std::cout << "Testing log file functionality... ";
    
    std::string testLogFile = "simple_test_log.txt";
    
    // Clean up any existing test log file (ignore errors)
    std::remove(testLogFile.c_str());
    
    AdaptiveCruiseControl acc(75.0, 70.0, 35.0, testLogFile);
    acc.saveStatusToFile();
    
    // Verify file was created
    {
        std::ifstream existsCheck(testLogFile.c_str());
        ASSERT_TRUE(existsCheck.good());
    }
    
    // Verify file content
    std::ifstream logFile(testLogFile);
    ASSERT_TRUE(logFile.is_open());
    
    std::string line;
    bool foundEgoSpeed = false;
    bool foundAheadSpeed = false;
    bool foundDistance = false;
    
    while (std::getline(logFile, line)) {
        if (line.find("Current Speed: 75.0 km/h") != std::string::npos) foundEgoSpeed = true;
        if (line.find("Car Ahead Speed: 70.0 km/h") != std::string::npos) foundAheadSpeed = true;
        if (line.find("Distance: 35.0 m") != std::string::npos) foundDistance = true;
    }
    
    logFile.close();
    
    ASSERT_TRUE(foundEgoSpeed);
    ASSERT_TRUE(foundAheadSpeed);
    ASSERT_TRUE(foundDistance);
    
    // Clean up (ignore errors)
    std::remove(testLogFile.c_str());
    
    std::cout << "PASS" << std::endl;
    return true;
}

bool testEdgeCases() {
    std::cout << "Testing edge cases... ";
    
    // Test very high speed
    AdaptiveCruiseControl acc1(200.0, 180.0, 100.0);
    double safeDistance = acc1.calculateSafeDistance();
    ASSERT_TRUE(safeDistance > 0.0);
    
    acc1.adjustSpeed();
    ASSERT_TRUE(acc1.getEgoSpeed() >= 0.0);
    
    // Test very small distance
    AdaptiveCruiseControl acc2(50.0, 45.0, 0.1);
    safeDistance = acc2.calculateSafeDistance();
    ASSERT_TRUE(acc2.getDistance() < safeDistance);
    
    acc2.adjustSpeed();
    ASSERT_TRUE(acc2.getEgoSpeed() <= 45.0);
    
    // Test zero speed — with a positive distance, algorithm increases by 2 km/h
    AdaptiveCruiseControl acc3(0.0, 50.0, 10.0);
    safeDistance = acc3.calculateSafeDistance();
    ASSERT_TRUE(acc3.getDistance() > safeDistance * 1.5);
    
    acc3.adjustSpeed();
    ASSERT_EQUAL(2.0, acc3.getEgoSpeed());
    
    std::cout << "PASS" << std::endl;
    return true;
}

int main() {
    std::cout << "=== Simple Unit Tests for Adaptive Cruise Control ===" << std::endl;
    std::cout << "Running tests..." << std::endl << std::endl;
    
    int passed = 0;
    int total = 0;
    
    // Run all tests
    if (testConstructor()) passed++; total++;
    if (testUpdateAheadVehicleSpeed()) passed++; total++;
    if (testUpdateDistance()) passed++; total++;
    if (testCalculateSafeDistance()) passed++; total++;
    if (testAdjustSpeed()) passed++; total++;
    if (testSpeedLimit()) passed++; total++;
    if (testLogFile()) passed++; total++;
    if (testEdgeCases()) passed++; total++;
    
    std::cout << std::endl;
    std::cout << "=== Test Results ===" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;
    
    if (passed == total) {
        std::cout << "All tests PASSED! 🎉" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAILED! ❌" << std::endl;
        return 1;
    }
}
