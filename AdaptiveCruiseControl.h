/**
 * @file AdaptiveCruiseControl.h
 * @brief Public interface for the Adaptive Cruise Control (ACC) component.
 *
 * Provides the `AdaptiveCruiseControl` class which models a simplified ACC
 * controller capable of tracking an ahead vehicle, computing a safe following
 * distance using the 2-second rule, and adjusting the ego vehicle's speed
 * accordingly. The class also supports console display and basic logging to a
 * text file for traceability.
 */

#ifndef ADAPTIVECRUISECONTROL_H
#define ADAPTIVECRUISECONTROL_H

#include <string>

/**
 * @class AdaptiveCruiseControl
 * @brief Simplified adaptive cruise control logic and telemetry logging.
 *
 * This class encapsulates the state and behavior of a basic ACC system:
 * - Tracks ego vehicle speed, ahead vehicle speed, and inter-vehicle distance
 * - Computes the safe following distance per the 2-second rule
 * - Adjusts ego speed to maintain a comfortable, capped, and safe gap
 * - Displays status to console and persists snapshots to a plain-text log file
 */
class AdaptiveCruiseControl {
private:
    double egoSpeed;              // Speed of the ego vehicle (km/h)
    double aheadVehicleSpeed;     // Speed of the vehicle ahead (km/h)
    double distanceToAheadVehicle; // Distance to the vehicle ahead (meters)
    std::string logFileName;      // Name of the log file

public:
    /**
     * @brief Constructs an ACC controller with initial conditions.
     * @param egoSpeed Initial ego vehicle speed in km/h.
     * @param aheadVehicleSpeed Initial ahead vehicle speed in km/h.
     * @param distance Initial distance to the ahead vehicle in meters.
     * @param logFile Path to a log file. When not provided, defaults to
     *        "acc_log.txt" in the working directory.
     */
    AdaptiveCruiseControl(double egoSpeed = 0.0,
                          double aheadVehicleSpeed = 0.0,
                          double distance = 0.0,
                          const std::string& logFile = "acc_log.txt");
    
    /**
     * @brief Updates the observed speed of the vehicle ahead.
     * @param speed Ahead vehicle speed in km/h. Negative values are ignored.
     */
    void updateAheadVehicleSpeed(double speed);

    /**
     * @brief Updates the measured distance to the vehicle ahead.
     * @param distance Current gap in meters. Negative values are ignored.
     */
    void updateDistance(double distance);

    /**
     * @brief Computes the safe following distance using the 2-second rule.
     *
     * Converts ego speed from km/h to m/s and multiplies by 2 seconds:
     * \f$\text{safeDistance} = v_{ego}\;[\text{km/h}] \times \frac{1000}{3600} \times 2
     * = v_{ego} \times \frac{5}{9}\;[\text{m}]\f$.
     *
     * @return Safe following distance in meters.
     */
    double calculateSafeDistance() const;

    /**
     * @brief Adjusts ego speed based on current gap and rule set.
     *
     * Behavior overview:
     * - If too close: reduce speed to match the ahead vehicle, or by 5 km/h if
     *   the ahead vehicle is faster, bounded below by 0.
     * - If the gap is comfortably large (> 1.5 × safe distance) and the ahead
     *   vehicle is faster: increase ego speed by up to 2 km/h, capped at 120 km/h.
     * - Otherwise: maintain current speed.
     */
    void adjustSpeed();

    /**
     * @brief Prints the current ACC status to the console.
     *
     * Includes ego speed, ahead vehicle speed, current distance, computed safe
     * distance, and a summarized advisory message.
     */
    void displayStatus() const;

    /**
     * @brief Appends a timestamped status snapshot to the configured log file.
     *
     * Writes human-readable key-value pairs that mirror the console output.
     * If the file cannot be opened, a message is printed to standard output.
     */
    void saveStatusToFile() const;
    
    /** @name Accessors
     *  @brief Read-only access to internal telemetry values.
     *  @{ */
    double getEgoSpeed() const { return egoSpeed; }
    double getAheadVehicleSpeed() const { return aheadVehicleSpeed; }
    double getDistance() const { return distanceToAheadVehicle; }
    /** @} */
    
    /**
     * @brief Sets the destination file path used by saveStatusToFile().
     * @param fileName Path to a writable text file. The file is opened in
     *        append mode when saving, and created if it does not exist.
     */
    void setLogFileName(const std::string& fileName) { logFileName = fileName; }

    /**
     * @brief Returns the currently configured log file path.
     * @return Path string to the log file.
     */
    std::string getLogFileName() const { return logFileName; }
};

#endif // ADAPTIVECRUISECONTROL_H
