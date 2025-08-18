#include <gtest/gtest.h>
#include "AdaptiveCruiseControl.h"
#include <fstream>
#include <sstream>
#include <filesystem>

class AdaptiveCruiseControlTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a test log file name that won't interfere with production logs
        testLogFile = "test_acc_log.txt";
        
        // Clean up any existing test log file
        if (std::filesystem::exists(testLogFile)) {
            std::filesystem::remove(testLogFile);
        }
    }
    
    void TearDown() override {
        // Clean up test log file after each test
        if (std::filesystem::exists(testLogFile)) {
            std::filesystem::remove(testLogFile);
        }
    }
    
    std::string testLogFile;
};

// Test Constructor
TEST_F(AdaptiveCruiseControlTest, ConstructorDefaultValues) {
    AdaptiveCruiseControl acc;
    
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), 0.0);
    EXPECT_DOUBLE_EQ(acc.getAheadVehicleSpeed(), 0.0);
    EXPECT_DOUBLE_EQ(acc.getDistance(), 0.0);
    EXPECT_EQ(acc.getLogFileName(), "acc_log.txt");
}

TEST_F(AdaptiveCruiseControlTest, ConstructorCustomValues) {
    AdaptiveCruiseControl acc(80.0, 75.0, 50.0, "custom_log.txt");
    
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), 80.0);
    EXPECT_DOUBLE_EQ(acc.getAheadVehicleSpeed(), 75.0);
    EXPECT_DOUBLE_EQ(acc.getDistance(), 50.0);
    EXPECT_EQ(acc.getLogFileName(), "custom_log.txt");
}

// Test updateAheadVehicleSpeed
TEST_F(AdaptiveCruiseControlTest, UpdateAheadVehicleSpeedValid) {
    AdaptiveCruiseControl acc;
    
    acc.updateAheadVehicleSpeed(60.0);
    EXPECT_DOUBLE_EQ(acc.getAheadVehicleSpeed(), 60.0);
    
    acc.updateAheadVehicleSpeed(0.0);
    EXPECT_DOUBLE_EQ(acc.getAheadVehicleSpeed(), 0.0);
}

TEST_F(AdaptiveCruiseControlTest, UpdateAheadVehicleSpeedInvalid) {
    AdaptiveCruiseControl acc(50.0, 45.0, 30.0);
    double originalSpeed = acc.getAheadVehicleSpeed();
    
    // Negative speed should not change the value
    acc.updateAheadVehicleSpeed(-10.0);
    EXPECT_DOUBLE_EQ(acc.getAheadVehicleSpeed(), originalSpeed);
}

// Test updateDistance
TEST_F(AdaptiveCruiseControlTest, UpdateDistanceValid) {
    AdaptiveCruiseControl acc;
    
    acc.updateDistance(25.0);
    EXPECT_DOUBLE_EQ(acc.getDistance(), 25.0);
    
    acc.updateDistance(0.0);
    EXPECT_DOUBLE_EQ(acc.getDistance(), 0.0);
}

TEST_F(AdaptiveCruiseControlTest, UpdateDistanceInvalid) {
    AdaptiveCruiseControl acc(50.0, 45.0, 30.0);
    double originalDistance = acc.getDistance();
    
    // Negative distance should not change the value
    acc.updateDistance(-5.0);
    EXPECT_DOUBLE_EQ(acc.getDistance(), originalDistance);
}

// Test calculateSafeDistance
TEST_F(AdaptiveCruiseControlTest, CalculateSafeDistanceZeroSpeed) {
    AdaptiveCruiseControl acc(0.0, 0.0, 0.0);
    
    double safeDistance = acc.calculateSafeDistance();
    EXPECT_DOUBLE_EQ(safeDistance, 0.0);
}

TEST_F(AdaptiveCruiseControlTest, CalculateSafeDistanceVariousSpeeds) {
    // Test various speeds and verify the 2-second rule calculation
    // Formula: speed * (5.0 / 9.0)
    
    AdaptiveCruiseControl acc1(60.0, 0.0, 0.0);
    double safeDistance1 = acc1.calculateSafeDistance();
    EXPECT_DOUBLE_EQ(safeDistance1, 60.0 * (5.0 / 9.0));
    
    AdaptiveCruiseControl acc2(120.0, 0.0, 0.0);
    double safeDistance2 = acc2.calculateSafeDistance();
    EXPECT_DOUBLE_EQ(safeDistance2, 120.0 * (5.0 / 9.0));
    
    AdaptiveCruiseControl acc3(30.0, 0.0, 0.0);
    double safeDistance3 = acc3.calculateSafeDistance();
    EXPECT_DOUBLE_EQ(safeDistance3, 30.0 * (5.0 / 9.0));
}

// Test adjustSpeed - Too close scenario
TEST_F(AdaptiveCruiseControlTest, AdjustSpeedTooClose) {
    AdaptiveCruiseControl acc(80.0, 70.0, 20.0, testLogFile);
    double initialSpeed = acc.getEgoSpeed();
    double safeDistance = acc.calculateSafeDistance();
    
    // Distance is less than safe distance
    EXPECT_LT(acc.getDistance(), safeDistance);
    
    acc.adjustSpeed();
    
    // Speed should be reduced to match ahead vehicle
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), 70.0);
}

TEST_F(AdaptiveCruiseControlTest, AdjustSpeedTooCloseEmergency) {
    AdaptiveCruiseControl acc(90.0, 85.0, 15.0, testLogFile);
    double initialSpeed = acc.getEgoSpeed();
    double safeDistance = acc.calculateSafeDistance();
    
    // Distance is much less than safe distance
    EXPECT_LT(acc.getDistance(), safeDistance);
    
    acc.adjustSpeed();
    
    // Speed should be reduced by 5 km/h
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), initialSpeed - 5.0);
}

// Test adjustSpeed - Safe distance scenario
TEST_F(AdaptiveCruiseControlTest, AdjustSpeedSafeDistance) {
    AdaptiveCruiseControl acc(60.0, 65.0, 50.0, testLogFile);
    double initialSpeed = acc.getEgoSpeed();
    double safeDistance = acc.calculateSafeDistance();
    
    // Distance is between safeDistance and 1.5*safeDistance
    EXPECT_GT(acc.getDistance(), safeDistance);
    EXPECT_LT(acc.getDistance(), safeDistance * 1.5);
    
    acc.adjustSpeed();
    
    // Speed should remain unchanged
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), initialSpeed);
}

// Test adjustSpeed - Good distance scenario
TEST_F(AdaptiveCruiseControlTest, AdjustSpeedGoodDistance) {
    AdaptiveCruiseControl acc(70.0, 80.0, 60.0, testLogFile);
    double initialSpeed = acc.getEgoSpeed();
    double safeDistance = acc.calculateSafeDistance();
    
    // Distance is greater than 1.5*safeDistance and ahead vehicle is faster
    EXPECT_GT(acc.getDistance(), safeDistance * 1.5);
    EXPECT_GT(acc.getAheadVehicleSpeed(), acc.getEgoSpeed());
    
    acc.adjustSpeed();
    
    // Speed should increase by 2 km/h
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), initialSpeed + 2.0);
}

// Test adjustSpeed - Speed limit enforcement
TEST_F(AdaptiveCruiseControlTest, AdjustSpeedSpeedLimit) {
    AdaptiveCruiseControl acc(118.0, 125.0, 80.0, testLogFile);
    double safeDistance = acc.calculateSafeDistance();
    
    // Distance is good and ahead vehicle is faster
    EXPECT_GT(acc.getDistance(), safeDistance * 1.5);
    EXPECT_GT(acc.getAheadVehicleSpeed(), acc.getEgoSpeed());
    
    acc.adjustSpeed();
    
    // Speed should not exceed 120 km/h
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), 120.0);
}

// Test adjustSpeed - Zero speed scenario
TEST_F(AdaptiveCruiseControlTest, AdjustSpeedZeroSpeed) {
    AdaptiveCruiseControl acc(0.0, 50.0, 10.0, testLogFile);
    double safeDistance = acc.calculateSafeDistance();
    
    // Distance is less than safe distance but ego speed is 0
    EXPECT_LT(acc.getDistance(), safeDistance);
    
    acc.adjustSpeed();
    
    // Speed should remain 0 (can't go negative)
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), 0.0);
}

// Test log file management
TEST_F(AdaptiveCruiseControlTest, SetAndGetLogFileName) {
    AdaptiveCruiseControl acc;
    
    std::string newLogFile = "new_test_log.txt";
    acc.setLogFileName(newLogFile);
    
    EXPECT_EQ(acc.getLogFileName(), newLogFile);
}

// Test saveStatusToFile
TEST_F(AdaptiveCruiseControlTest, SaveStatusToFile) {
    AdaptiveCruiseControl acc(75.0, 70.0, 35.0, testLogFile);
    
    // Save status to file
    acc.saveStatusToFile();
    
    // Verify file was created and contains expected content
    EXPECT_TRUE(std::filesystem::exists(testLogFile));
    
    std::ifstream logFile(testLogFile);
    ASSERT_TRUE(logFile.is_open());
    
    std::string line;
    bool foundTimestamp = false;
    bool foundEgoSpeed = false;
    bool foundAheadSpeed = false;
    bool foundDistance = false;
    
    while (std::getline(logFile, line)) {
        if (line.find("Timestamp:") != std::string::npos) foundTimestamp = true;
        if (line.find("Current Speed: 75.0 km/h") != std::string::npos) foundEgoSpeed = true;
        if (line.find("Car Ahead Speed: 70.0 km/h") != std::string::npos) foundAheadSpeed = true;
        if (line.find("Distance: 35.0 m") != std::string::npos) foundDistance = true;
    }
    
    logFile.close();
    
    EXPECT_TRUE(foundTimestamp);
    EXPECT_TRUE(foundEgoSpeed);
    EXPECT_TRUE(foundAheadSpeed);
    EXPECT_TRUE(foundDistance);
}

// Test saveStatusToFile with invalid filename
TEST_F(AdaptiveCruiseControlTest, SaveStatusToFileInvalidPath) {
    AdaptiveCruiseControl acc(75.0, 70.0, 35.0, "/invalid/path/test.txt");
    
    // This should not crash and should handle the error gracefully
    EXPECT_NO_THROW(acc.saveStatusToFile());
}

// Test edge cases
TEST_F(AdaptiveCruiseControlTest, EdgeCaseVeryHighSpeed) {
    AdaptiveCruiseControl acc(200.0, 180.0, 100.0, testLogFile);
    
    double safeDistance = acc.calculateSafeDistance();
    EXPECT_GT(safeDistance, 0.0);
    
    acc.adjustSpeed();
    // Should handle high speeds gracefully
    EXPECT_GE(acc.getEgoSpeed(), 0.0);
}

TEST_F(AdaptiveCruiseControlTest, EdgeCaseVerySmallDistance) {
    AdaptiveCruiseControl acc(50.0, 45.0, 0.1, testLogFile);
    
    double safeDistance = acc.calculateSafeDistance();
    EXPECT_LT(acc.getDistance(), safeDistance);
    
    acc.adjustSpeed();
    // Should reduce speed when very close
    EXPECT_LE(acc.getEgoSpeed(), 45.0);
}

// Test multiple consecutive adjustments
TEST_F(AdaptiveCruiseControlTest, MultipleAdjustments) {
    AdaptiveCruiseControl acc(80.0, 70.0, 25.0, testLogFile);
    
    // First adjustment
    acc.adjustSpeed();
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), 70.0);
    
    // Update distance to be safe
    acc.updateDistance(50.0);
    
    // Second adjustment
    acc.adjustSpeed();
    // Speed should remain at 70 since distance is now safe
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), 70.0);
    
    // Update ahead vehicle to be faster
    acc.updateAheadVehicleSpeed(85.0);
    
    // Third adjustment
    acc.adjustSpeed();
    // Speed should increase since distance is good and ahead vehicle is faster
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), 72.0);
}

// Test boundary conditions
TEST_F(AdaptiveCruiseControlTest, BoundaryConditions) {
    AdaptiveCruiseControl acc(60.0, 55.0, 30.0, testLogFile);
    double safeDistance = acc.calculateSafeDistance();
    
    // Test exactly at safe distance
    acc.updateDistance(safeDistance);
    double speedBefore = acc.getEgoSpeed();
    acc.adjustSpeed();
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), speedBefore);
    
    // Test exactly at 1.5 * safe distance
    acc.updateDistance(safeDistance * 1.5);
    speedBefore = acc.getEgoSpeed();
    acc.adjustSpeed();
    EXPECT_DOUBLE_EQ(acc.getEgoSpeed(), speedBefore);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
