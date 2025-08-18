# Adaptive Cruise Control System

A console-based C++ implementation of an Adaptive Cruise Control (ACC) system that automatically adjusts vehicle speed to maintain safe following distance using the 2-second rule.

## Features

- **Safe Distance Calculation**: Implements the 2-second rule for determining safe following distance
- **Automatic Speed Adjustment**: Dynamically adjusts ego vehicle speed based on traffic conditions
- **Real-time Monitoring**: Continuously monitors vehicle speeds and distances
- **Safety Alerts**: Provides warnings when vehicles are too close
- **Console Interface**: Clean, informative console output with status updates
- **Interactive Mode**: Test custom scenarios by entering your own values
- **Demo Mode**: Pre-configured scenarios demonstrating system functionality

## Class Structure

### AdaptiveCruiseControl Class

**Data Members:**
- `egoSpeed` - Speed of the ego vehicle (km/h)
- `aheadVehicleSpeed` - Speed of the vehicle ahead (km/h)
- `distanceToAheadVehicle` - Distance to the vehicle ahead (meters)

**Member Functions:**
- Constructor - Initializes speeds and distance
- `updateAheadVehicleSpeed(double speed)` - Sets the speed of the vehicle ahead
- `updateDistance(double distance)` - Updates current distance to the vehicle ahead
- `calculateSafeDistance() const` - Calculates safe following distance using the 2-second rule
- `adjustSpeed()` - Adjusts ego vehicle speed based on the gap
- `displayStatus() const` - Displays current speeds, distances, and alerts

## How It Works

### 2-Second Rule
The system calculates safe following distance using the 2-second rule:
- Safe Distance = Current Speed × (5/9) meters
- This ensures the ego vehicle can stop safely if the vehicle ahead brakes suddenly

### Speed Adjustment Logic
1. **Too Close**: If distance < safe distance, reduce speed to match ahead vehicle
2. **Safe Distance**: If distance is within safe range, maintain current speed
3. **Good Distance**: If distance > 1.5× safe distance, can increase speed slightly

## Usage Modes

### Demo Mode
Pre-configured scenarios that demonstrate:
- Initial unsafe conditions (vehicle too close)
- Speed adjustment response
- Safe distance achievement
- Dynamic updates and emergency scenarios

### Interactive Mode
Test your own scenarios by entering:
- **Ego Vehicle Speed**: 0-120 km/h
- **Ahead Vehicle Speed**: 0-120 km/h  
- **Distance**: 0-200 meters

The system will:
- Validate your inputs
- Display current status and safety analysis
- Optionally adjust speed based on conditions
- Allow testing multiple scenarios

## Compilation and Execution

### Prerequisites
- C++ compiler with C++11 support (GCC, Clang, or MSVC)
- Make utility (optional, for using Makefile)

### Using Makefile (Recommended)
```bash
# Build the project
make

# Run the program
make run

# Clean build files
make clean

# Show help
make help
```

### Manual Compilation
```bash
# Compile all source files
g++ -std=c++11 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system

# Run the executable
./acc_system
```

### Windows Compilation
```cmd
# Using MinGW
g++ -std=c++11 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system.exe

# Run the executable
acc_system.exe
```

## Example Output

### Demo Mode
```
=== Adaptive Cruise Control System - Demo Mode ===
Simulating vehicle following scenarios...

Scenario 1: Vehicle too close
Current Speed: 60.0 km/h
Car Ahead Speed: 55.0 km/h
Distance: 25.0 m
Safe Distance: 33.3 m
⚠ Too close! Reduce speed to 55.0 km/h
-------------------------------------
```

### Interactive Mode
```
=== Adaptive Cruise Control System - Interactive Mode ===
Enter your own values to test different scenarios

Enter ego vehicle speed (km/h, 0-120): 80
Enter ahead vehicle speed (km/h, 0-120): 75
Enter distance to ahead vehicle (meters, 0-200): 45

=== Your Scenario ===
Current Speed: 80.0 km/h
Car Ahead Speed: 75.0 km/h
Distance: 45.0 m
Safe Distance: 44.4 m
⚠ Caution — Maintain current speed
-------------------------------------
```

## Project Structure

```
ACC/
├── AdaptiveCruiseControl.h    # Header file with class declaration
├── AdaptiveCruiseControl.cpp  # Implementation file
├── main.cpp                   # Main program with menu and interactive modes
├── Makefile                   # Build configuration
└── README.md                  # This file
```

## Safety Features

- **Input Validation**: All speed and distance inputs are validated to be non-negative
- **Speed Limits**: Maximum speed is capped at 120 km/h for safety
- **Gradual Adjustments**: Speed changes are made gradually to avoid sudden movements
- **Clear Warnings**: Visual indicators (⚠, ✅) for different safety levels
- **Range Validation**: Inputs are constrained to realistic ranges (0-120 km/h, 0-200m)

## Future Enhancements

- Real-time sensor data integration
- Weather condition adjustments
- Traffic pattern learning
- Emergency braking simulation
- Graphical user interface
- Data logging and analysis

## License

This project is open source and available under the MIT License.

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve the system.
