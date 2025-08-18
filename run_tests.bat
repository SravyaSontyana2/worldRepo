@echo off
echo ========================================
echo Adaptive Cruise Control - Test Runner
echo ========================================
echo.

echo Building and running simple tests...
echo.

REM Build the simple tests
make simple_tests.exe

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Failed to build tests!
    echo Please check your compiler installation and try again.
    pause
    exit /b 1
)

echo.
echo Tests built successfully! Running tests...
echo.

REM Run the tests
simple_tests.exe

echo.
echo ========================================
echo Test execution completed.
echo ========================================
pause
