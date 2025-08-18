# Adaptive Cruise Control - Test Runner (PowerShell)
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Adaptive Cruise Control - Test Runner" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if g++ is available
try {
    $gppVersion = g++ --version 2>$null
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ g++ compiler found" -ForegroundColor Green
    } else {
        throw "g++ not found"
    }
} catch {
    Write-Host "✗ g++ compiler not found!" -ForegroundColor Red
    Write-Host "Please install MinGW-w64 or another C++ compiler." -ForegroundColor Yellow
    Write-Host "Download from: https://www.mingw-w64.org/" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""
Write-Host "Building simple tests..." -ForegroundColor Yellow

# Compile the simple tests
try {
    # Compile source files
    Write-Host "Compiling AdaptiveCruiseControl.cpp..." -ForegroundColor Gray
    g++ -std=c++17 -Wall -Wextra -O2 -c AdaptiveCruiseControl.cpp -o AdaptiveCruiseControl.o
    
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to compile AdaptiveCruiseControl.cpp"
    }
    
    Write-Host "Compiling simple_test.cpp..." -ForegroundColor Gray
    g++ -std=c++17 -Wall -Wextra -O2 -c simple_test.cpp -o simple_test.o
    
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to compile simple_test.cpp"
    }
    
    # Link the executable
    Write-Host "Linking test executable..." -ForegroundColor Gray
    g++ simple_test.o AdaptiveCruiseControl.o -o simple_tests.exe
    
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to link test executable"
    }
    
    Write-Host "✓ Tests built successfully!" -ForegroundColor Green
    
} catch {
    Write-Host ""
    Write-Host "ERROR: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "Please check your compiler installation and try again." -ForegroundColor Yellow
    
    # Clean up object files
    if (Test-Path "*.o") { Remove-Item "*.o" }
    if (Test-Path "simple_tests.exe") { Remove-Item "simple_tests.exe" }
    
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""
Write-Host "Running tests..." -ForegroundColor Yellow
Write-Host ""

# Run the tests
try {
    & ".\simple_tests.exe"
    $testExitCode = $LASTEXITCODE
} catch {
    Write-Host "ERROR: Failed to run tests" -ForegroundColor Red
    $testExitCode = 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Test execution completed." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# Clean up object files
if (Test-Path "*.o") { 
    Write-Host "Cleaning up object files..." -ForegroundColor Gray
    Remove-Item "*.o" 
}

if ($testExitCode -eq 0) {
    Write-Host "✓ All tests passed!" -ForegroundColor Green
} else {
    Write-Host "✗ Some tests failed!" -ForegroundColor Red
}

Read-Host "Press Enter to exit"
exit $testExitCode
