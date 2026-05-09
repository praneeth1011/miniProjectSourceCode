@echo off
REM Banking System - Windows Setup and Build Script

echo.
echo ==================================================
echo Multi-User ATM Banking System - Windows Setup
echo ==================================================
echo.

REM Check for GCC
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo Error: GCC not found in PATH
    echo Please install MinGW or GCC for Windows
    echo Visit: https://www.mingw-w64.org/
    pause
    exit /b 1
)

echo [OK] GCC found

REM Create bin directory if it doesn't exist
if not exist bin mkdir bin

REM Clean previous builds
echo.
echo Cleaning previous builds...
if exist banking_system.exe del banking_system.exe
if exist *.o del *.o

REM Compile all source files
echo.
echo Compiling source files...
gcc -Wall -Wextra -std=c99 -c banking.c -o banking.o
if %errorlevel% neq 0 goto :error

gcc -Wall -Wextra -std=c99 -c auth.c -o auth.o
if %errorlevel% neq 0 goto :error

gcc -Wall -Wextra -std=c99 -c fileops.c -o fileops.o
if %errorlevel% neq 0 goto :error

gcc -Wall -Wextra -std=c99 -c transactions.c -o transactions.o
if %errorlevel% neq 0 goto :error

gcc -Wall -Wextra -std=c99 -c admin.c -o admin.o
if %errorlevel% neq 0 goto :error

gcc -Wall -Wextra -std=c99 -c utils.c -o utils.o
if %errorlevel% neq 0 goto :error

REM Link all object files
echo.
echo Linking object files...
gcc -o banking_system banking.o auth.o fileops.o transactions.o admin.o utils.o -lm
if %errorlevel% neq 0 goto :error

REM Success
echo.
echo ==================================================
echo [SUCCESS] Compilation completed!
echo ==================================================
echo.
echo Ready to run: banking_system.exe
echo.
echo Options:
echo   1. Run the program now (type: go)
echo   2. Compile only (type: exit and run banking_system.exe)
echo.
echo Type your choice: 
set /p choice=
if /i "%choice%"=="go" (
    cls
    banking_system.exe
) else (
    echo.
    echo To run the program later, type: banking_system.exe
    echo.
)
exit /b 0

:error
echo.
echo [ERROR] Compilation failed!
echo Please check for syntax errors in the source files.
pause
exit /b 1
