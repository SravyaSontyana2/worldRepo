/**
 * @file main.cpp
 * @brief Interactive demo and CLI utilities for the ACC system.
 *
 * Provides a demonstration flow that showcases the Adaptive Cruise Control
 * behavior in multiple scenarios and basic log viewing helpers for
 * human-readable inspection.
 */

#include "AdaptiveCruiseControl.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <string>

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void viewLogFile(const std::string& fileName) {
    clearScreen();
    std::cout << "=== Viewing Log File: " << fileName << " ===" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    std::ifstream logFile(fileName);
    if (logFile.is_open()) {
        std::string line;
        while (std::getline(logFile, line)) {
            std::cout << line << std::endl;
        }
        logFile.close();
    } else {
        std::cout << "No log file found or file is empty." << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void runDemo() {
    std::cout << "=== Adaptive Cruise Control System - Demo Mode ===" << std::endl;
    std::cout << "Simulating vehicle following scenarios..." << std::endl << std::endl;
    
    // Create ACC system with initial values and logging
    AdaptiveCruiseControl acc(60.0, 55.0, 25.0, "demo_log.txt");
    
    std::cout << "Scenario 1: Vehicle too close" << std::endl;
    acc.displayStatus();
    acc.saveStatusToFile(); // Save to log
    
    // Adjust speed based on current situation
    acc.adjustSpeed();
    
    std::cout << "After speed adjustment:" << std::endl;
    acc.displayStatus();
    acc.saveStatusToFile(); // Save to log
    
    // Update distance to show improvement
    acc.updateDistance(34.0);
    
    std::cout << "Scenario 2: Safe distance achieved" << std::endl;
    acc.displayStatus();
    acc.saveStatusToFile(); // Save to log
    
    // Demonstrate dynamic updates
    std::cout << "\n=== Dynamic Updates Demo ===" << std::endl;
    
    // Update ahead vehicle speed
    acc.updateAheadVehicleSpeed(70.0);
    acc.updateDistance(40.0);
    
    std::cout << "Updated scenario:" << std::endl;
    acc.displayStatus();
    acc.saveStatusToFile(); // Save to log
    
    // Adjust speed for new conditions
    acc.adjustSpeed();
    
    std::cout << "After adjustment for new conditions:" << std::endl;
    acc.displayStatus();
    acc.saveStatusToFile(); // Save to log
    
    // Emergency scenario
    std::cout << "\n=== Emergency Scenario ===" << std::endl;
    acc.updateDistance(15.0); // Very close!
    acc.displayStatus();
    acc.saveStatusToFile(); // Save to log
    
    acc.adjustSpeed();
    
    std::cout << "Emergency response:" << std::endl;
    acc.displayStatus();
    acc.saveStatusToFile(); // Save to log
    
    std::cout << "\n=== System Summary ===" << std::endl;
    std::cout << "Final Status:" << std::endl;
    acc.displayStatus();
    acc.saveStatusToFile(); // Save to log
    
    std::cout << "\nDemo completed. All scenarios have been logged to: " << acc.getLogFileName() << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void runInteractive() {
    char continueChoice;
    std::string logFileName;
    
    std::cout << "Enter log file name (or press Enter for default 'interactive_log.txt'): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, logFileName);
    
    if (logFileName.empty()) {
        logFileName = "interactive_log.txt";
    }
    
    do {
        clearScreen();
        std::cout << "=== Adaptive Cruise Control System - Interactive Mode ===" << std::endl;
        std::cout << "Logging to: " << logFileName << std::endl;
        std::cout << "Enter your own values to test different scenarios" << std::endl << std::endl;
        
        double egoSpeed, aheadSpeed, distance;
        
        // Get ego vehicle speed
        do {
            std::cout << "Enter ego vehicle speed (km/h, 0-120): ";
            if (!(std::cin >> egoSpeed) || egoSpeed < 0 || egoSpeed > 120) {
                std::cout << "Invalid input! Please enter a value between 0 and 120." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } while (egoSpeed < 0 || egoSpeed > 120);
        
        // Get ahead vehicle speed
        do {
            std::cout << "Enter ahead vehicle speed (km/h, 0-120): ";
            if (!(std::cin >> aheadSpeed) || aheadSpeed < 0 || aheadSpeed > 120) {
                std::cout << "Invalid input! Please enter a value between 0 and 120." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } while (aheadSpeed < 0 || aheadSpeed > 120);
        
        // Get distance
        do {
            std::cout << "Enter distance to ahead vehicle (meters, 0-200): ";
            if (!(std::cin >> distance) || distance < 0 || distance > 200) {
                std::cout << "Invalid input! Please enter a value between 0 and 200." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } while (distance < 0 || distance > 200);
        
        std::cout << "\n=== Your Scenario ===" << std::endl;
        
        // Create ACC system with user values and logging
        AdaptiveCruiseControl acc(egoSpeed, aheadSpeed, distance, logFileName);
        acc.displayStatus();
        acc.saveStatusToFile(); // Save to log
        
        // Ask if user wants to adjust speed
        char adjustChoice;
        std::cout << "\nWould you like to adjust speed based on current conditions? (y/n): ";
        std::cin >> adjustChoice;
        
        if (adjustChoice == 'y' || adjustChoice == 'Y') {
            acc.adjustSpeed();
            std::cout << "\nAfter speed adjustment:" << std::endl;
            acc.displayStatus();
            acc.saveStatusToFile(); // Save to log
        }
        
        // Ask if user wants to continue
        std::cout << "\nWould you like to test another scenario? (y/n): ";
        std::cin >> continueChoice;
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
    } while (continueChoice == 'y' || continueChoice == 'Y');
    
    std::cout << "\nAll scenarios have been logged to: " << logFileName << std::endl;
}

void showMenu() {
    int choice;
    
    do {
        clearScreen();
        std::cout << "=== Adaptive Cruise Control System ===" << std::endl;
        std::cout << "1. Run Demo Mode (Pre-configured scenarios)" << std::endl;
        std::cout << "2. Interactive Mode (Enter your own values)" << std::endl;
        std::cout << "3. View Log Files" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "\nEnter your choice (1-4): ";
        
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input! Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = 0;
        }
        
        switch (choice) {
            case 1:
                clearScreen();
                runDemo();
                break;
                
            case 2:
                runInteractive();
                break;
                
            case 3:
                {
                    std::string fileName;
                    std::cout << "Enter log file name to view (or press Enter for default 'acc_log.txt'): ";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, fileName);
                    
                    if (fileName.empty()) {
                        fileName = "acc_log.txt";
                    }
                    viewLogFile(fileName);
                }
                break;
                
            case 4:
                std::cout << "\nThank you for using Adaptive Cruise Control System!" << std::endl;
                std::cout << "Check the log files for a complete record of all sessions." << std::endl;
                break;
                
            default:
                if (choice != 0) {
                    std::cout << "Invalid choice! Please enter 1, 2, 3, or 4." << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                }
        }
        
    } while (choice != 4);
}

int main() {
    std::cout << "Welcome to Adaptive Cruise Control System!" << std::endl;
    std::cout << "This system implements the 2-second rule for safe following distance." << std::endl;
    std::cout << "All sessions will be automatically logged for record keeping." << std::endl << std::endl;
    
    showMenu();
    
    return 0;
}
