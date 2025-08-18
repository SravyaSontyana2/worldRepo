# Compilation Guide for Adaptive Cruise Control System

This guide provides step-by-step instructions for compiling the project and tests without using Make or CMake.

## Prerequisites

- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **Windows**: MinGW-w64, Visual Studio, or MSYS2
- **Linux/macOS**: GCC or Clang (usually pre-installed)

## Quick Compilation Commands

### 1. Compile Main Program

```bash
# Single command compilation
g++ -std=c++17 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system

# Or compile separately then link
g++ -std=c++17 -Wall -Wextra -O2 -c main.cpp -o main.o
g++ -std=c++17 -Wall -Wextra -O2 -c AdaptiveCruiseControl.cpp -o AdaptiveCruiseControl.o
g++ main.o AdaptiveCruiseControl.o -o acc_system
```

### 2. Compile Simple Tests

```bash
# Single command compilation
g++ -std=c++17 -Wall -Wextra -O2 simple_test.cpp AdaptiveCruiseControl.cpp -o simple_tests

# Or compile separately then link
g++ -std=c++17 -Wall -Wextra -O2 -c simple_test.cpp -o simple_test.o
g++ -std=c++17 -Wall -Wextra -O2 -c AdaptiveCruiseControl.cpp -o AdaptiveCruiseControl.o
g++ simple_test.o AdaptiveCruiseControl.o -o simple_tests
```

### 3. Compile Google Test (if available)

```bash
# Install Google Test first, then compile
g++ -std=c++17 -Wall -Wextra -O2 test_AdaptiveCruiseControl.cpp AdaptiveCruiseControl.cpp -lgtest -lgtest_main -lpthread -o acc_tests
```

## Platform-Specific Instructions

### Windows

#### Option 1: MinGW-w64
```cmd
# Install MinGW-w64 from https://www.mingw-w64.org/
# Add to PATH, then use:
g++ -std=c++17 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system.exe
```

#### Option 2: Visual Studio Developer Command Prompt
```cmd
# Open "Developer Command Prompt for VS"
cl /std:c++17 /EHsc /W3 main.cpp AdaptiveCruiseControl.cpp /Fe:acc_system.exe
```

#### Option 3: MSYS2
```bash
# Install MSYS2, then in MSYS2 terminal:
pacman -S mingw-w64-x86_64-gcc
g++ -std=c++17 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system.exe
```

### Linux

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential

# Compile
g++ -std=c++17 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system

# Run
./acc_system
```

### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Compile
g++ -std=c++17 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system

# Run
./acc_system
```

## Running Tests

### Simple Tests
```bash
# Compile and run
g++ -std=c++17 -Wall -Wextra -O2 simple_test.cpp AdaptiveCruiseControl.cpp -o simple_tests
./simple_tests          # Linux/macOS
simple_tests.exe        # Windows
```

### Google Test
```bash
# Install Google Test first, then:
g++ -std=c++17 -Wall -Wextra -O2 test_AdaptiveCruiseControl.cpp AdaptiveCruiseControl.cpp -lgtest -lgtest_main -lpthread -o acc_tests
./acc_tests             # Linux/macOS
acc_tests.exe           # Windows
```

## Compiler Flags Explained

- `-std=c++17`: Use C++17 standard (required for std::filesystem)
- `-Wall`: Enable all common warnings
- `-Wextra`: Enable extra warnings
- `-O2`: Optimize for performance
- `-c`: Compile only (don't link)
- `-o filename`: Specify output filename

## Troubleshooting

### Common Compilation Errors

#### 1. "std::filesystem not found"
```bash
# Use C++17 or later
g++ -std=c++17 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system
```

#### 2. "g++ not found"
- **Windows**: Install MinGW-w64 or use Visual Studio
- **Linux**: `sudo apt-get install build-essential`
- **macOS**: Install Xcode Command Line Tools

#### 3. "Permission denied"
```bash
# Make executable (Linux/macOS)
chmod +x acc_system
```

#### 4. "Missing header files"
- Ensure all `.cpp` and `.h` files are in the same directory
- Check include paths in source files

### Performance Optimization

```bash
# Debug build (no optimization)
g++ -std=c++17 -Wall -Wextra -g -O0 main.cpp AdaptiveCruiseControl.cpp -o acc_system_debug

# Release build (maximum optimization)
g++ -std=c++17 -Wall -Wextra -O3 -DNDEBUG main.cpp AdaptiveCruiseControl.cpp -o acc_system_release

# Profile-guided optimization
g++ -std=c++17 -Wall -Wextra -fprofile-generate main.cpp AdaptiveCruiseControl.cpp -o acc_system
./acc_system  # Run to generate profile
g++ -std=c++17 -Wall -Wextra -fprofile-use -fprofile-correction main.cpp AdaptiveCruiseControl.cpp -o acc_system_optimized
```

## IDE Integration

### Visual Studio Code
1. Install C/C++ extension
2. Create `.vscode/tasks.json`:
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "g++",
            "args": ["-std=c++17", "-Wall", "-Wextra", "-O2", "main.cpp", "AdaptiveCruiseControl.cpp", "-o", "acc_system"],
            "group": "build"
        }
    ]
}
```

### CLion
1. Open project folder
2. CLion will auto-detect C++ project
3. Use Build → Build Project (Ctrl+F9)

## Batch Compilation Scripts

### Windows (batch file)
```batch
@echo off
echo Compiling Adaptive Cruise Control System...
g++ -std=c++17 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system.exe
if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
    echo Running program...
    acc_system.exe
) else (
    echo Compilation failed!
)
pause
```

### Linux/macOS (shell script)
```bash
#!/bin/bash
echo "Compiling Adaptive Cruise Control System..."
g++ -std=c++17 -Wall -Wextra -O2 main.cpp AdaptiveCruiseControl.cpp -o acc_system
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Running program..."
    ./acc_system
else
    echo "Compilation failed!"
fi
```

## Next Steps

1. **Compile the main program** using the commands above
2. **Run the program** to verify it works
3. **Compile and run tests** to verify functionality
4. **Modify the code** as needed
5. **Recompile** after changes

## Getting Help

- Check compiler error messages carefully
- Verify C++ standard compatibility
- Ensure all dependencies are installed
- Use `-v` flag for verbose compilation output:
  ```bash
  g++ -v -std=c++17 main.cpp AdaptiveCruiseControl.cpp -o acc_system
  ```
