/**
 * @file AdaptiveCruiseControl.cpp
 * @brief Implementation of the Adaptive Cruise Control (ACC) component.
 *
 * Contains the operational logic for updating telemetry, computing safe
 * following distance using the 2-second rule, adjusting the ego vehicle's
 * speed, and persisting status snapshots to a log file.
 */

#include "AdaptiveCruiseControl.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cstring>

// Constructor - Initializes speeds and distance
/**
 * @brief Constructs an ACC controller with initial state.
 * @param egoSpeed Initial ego vehicle speed in km/h.
 * @param aheadVehicleSpeed Initial ahead vehicle speed in km/h.
 * @param distance Initial distance to the ahead vehicle in meters.
 * @param logFile Path to the log file used by saveStatusToFile().
 */
AdaptiveCruiseControl::AdaptiveCruiseControl(double egoSpeed, double aheadVehicleSpeed, double distance, const std::string& logFile) 
    : egoSpeed(egoSpeed), aheadVehicleSpeed(aheadVehicleSpeed), distanceToAheadVehicle(distance), logFileName(logFile) {
}

// Sets the speed of the vehicle ahead
/**
 * @brief Updates the observed speed of the vehicle ahead.
 * @param speed Ahead vehicle speed in km/h. Negative inputs are ignored.
 */
void AdaptiveCruiseControl::updateAheadVehicleSpeed(double speed) {
    if (speed >= 0.0) {
        aheadVehicleSpeed = speed;
    }
}

// Updates current distance to the vehicle ahead
/**
 * @brief Updates the measured gap to the ahead vehicle.
 * @param distance Current gap in meters. Negative inputs are ignored.
 */
void AdaptiveCruiseControl::updateDistance(double distance) {
    if (distance >= 0.0) {
        distanceToAheadVehicle = distance;
    }
}

// Calculates safe following distance using the 2-second rule
/**
 * @brief Computes the safe following distance using the 2-second rule.
 *
 * Converts ego speed from km/h to m/s and multiplies by 2 seconds:
 * \f$\text{safeDistance} = v_{ego}\;[\text{km/h}] \times \frac{1000}{3600} \times 2
 * = v_{ego} \times \frac{5}{9}\;[\text{m}]\f$.
 *
 * @return Safe following distance in meters.
 */
double AdaptiveCruiseControl::calculateSafeDistance() const {
    // 2-second rule: safe distance = speed * (2/3600) * 1000 meters
    // Convert km/h to m/s: 1 km/h = 1000/3600 m/s
    // Safe distance = speed * (2/3600) * 1000 = speed * (2000/3600) = speed * (5/9)
    return egoSpeed * (5.0 / 9.0);
}

// Adjusts ego vehicle speed based on the gap
/**
 * @brief Adjusts the ego vehicle speed to encourage a safe following gap.
 *
 * Behavior:
 * - If the gap is smaller than safe: reduce speed to the ahead vehicle's speed
 *   if it is slower, otherwise reduce ego speed by 5 km/h (bounded below by 0).
 * - If the gap is greater than 1.5 × safe and the ahead vehicle is faster:
 *   increase ego speed by up to 2 km/h, capped at 120 km/h.
 * - Otherwise, maintain current speed.
 */
void AdaptiveCruiseControl::adjustSpeed() {
    double safeDistance = calculateSafeDistance();
    
    if (distanceToAheadVehicle < safeDistance) {
        // Too close - reduce speed to match ahead vehicle or maintain safe distance
        if (aheadVehicleSpeed < egoSpeed) {
            egoSpeed = aheadVehicleSpeed;
        } else {
            // If ahead vehicle is faster but we're too close, reduce speed slightly
            egoSpeed = std::max(0.0, egoSpeed - 5.0);
        }
    } else if (distanceToAheadVehicle > safeDistance * 1.5) {
        // Good distance - can increase speed slightly if ahead vehicle is faster
        if (aheadVehicleSpeed > egoSpeed && egoSpeed < 120.0) { // Max speed limit
            egoSpeed = std::min(120.0, egoSpeed + 2.0);
        }
    }
    // If distance is safe (between safeDistance and 1.5*safeDistance), maintain current speed
}

// Displays current speeds, distances, and alerts
/**
 * @brief Prints the current ACC telemetry and advisory to standard output.
 *
 * Includes ego speed, ahead speed, actual and safe distance, and a succinct
 * advisory message to reduce speed, hold speed, or exercise caution.
 */
void AdaptiveCruiseControl::displayStatus() const {
    double safeDistance = calculateSafeDistance();
    
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Current Speed: " << egoSpeed << " km/h" << std::endl;
    std::cout << "Car Ahead Speed: " << aheadVehicleSpeed << " km/h" << std::endl;
    std::cout << "Distance: " << distanceToAheadVehicle << " m" << std::endl;
    std::cout << "Safe Distance: " << safeDistance << " m" << std::endl;
    
    if (distanceToAheadVehicle < safeDistance) {
        std::cout << "⚠ Too close! Reduce speed to " << aheadVehicleSpeed << " km/h" << std::endl;
    } else if (distanceToAheadVehicle > safeDistance * 1.2) {
        std::cout << "✅ Safe gap maintained — Hold speed" << std::endl;
    } else {
        std::cout << "⚠ Caution — Maintain current speed" << std::endl;
    }
    std::cout << "-------------------------------------" << std::endl;
}

// Saves current status to log file with timestamp
/**
 * @brief Appends a timestamped status snapshot to the configured log file.
 *
 * The log contains key telemetry values and a brief status line that mirrors
 * the console output. If the file cannot be opened, an error is printed.
 */
void AdaptiveCruiseControl::saveStatusToFile() const {
    std::ofstream logFile(logFileName, std::ios::app); // Open in append mode
    
    if (logFile.is_open()) {
        // Get current timestamp
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt) - 1] = '\0'; // Remove newline from ctime
        
        double safeDistance = calculateSafeDistance();
        
        logFile << std::fixed << std::setprecision(1);
        logFile << "=== ACC Status Record ===" << std::endl;
        logFile << "Timestamp: " << dt << std::endl;
        logFile << "Current Speed: " << egoSpeed << " km/h" << std::endl;
        logFile << "Car Ahead Speed: " << aheadVehicleSpeed << " km/h" << std::endl;
        logFile << "Distance: " << distanceToAheadVehicle << " m" << std::endl;
        logFile << "Safe Distance: " << safeDistance << " m" << std::endl;
        
        if (distanceToAheadVehicle < safeDistance) {
            logFile << "Status: Too close! Reduce speed to " << aheadVehicleSpeed << " km/h" << std::endl;
        } else if (distanceToAheadVehicle > safeDistance * 1.2) {
            logFile << "Status: Safe gap maintained — Hold speed" << std::endl;
        } else {
            logFile << "Status: Caution — Maintain current speed" << std::endl;
        }
        logFile << "-------------------------------------" << std::endl;
        
        logFile.close();
        std::cout << "Status saved to log file: " << logFileName << std::endl;
    } else {
        std::cout << "Error: Could not open log file for writing!" << std::endl;
    }
}
