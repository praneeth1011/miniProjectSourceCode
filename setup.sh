#!/bin/bash
# Quick Setup and Test Script for Banking System

echo "=================================================="
echo "Multi-User ATM Banking System - Setup Script"
echo "=================================================="
echo ""

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if running on Windows
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    echo -e "${YELLOW}Windows detected. Use mingw32-make or gcc directly.${NC}"
    echo "Command: gcc -o banking_system *.c -lm"
    exit 0
fi

# Check for GCC
if ! command -v gcc &> /dev/null; then
    echo -e "${RED}Error: GCC not found. Please install GCC.${NC}"
    exit 1
fi

echo -e "${GREEN}[✓] GCC found${NC}"

# Check for Make
if ! command -v make &> /dev/null; then
    echo -e "${RED}Error: Make not found. Please install Make.${NC}"
    exit 1
fi

echo -e "${GREEN}[✓] Make found${NC}"

# Clean previous builds
echo ""
echo "Cleaning previous builds..."
make clean 2>/dev/null

# Compile the program
echo ""
echo "Compiling the banking system..."
if make; then
    echo -e "${GREEN}[✓] Compilation successful!${NC}"
else
    echo -e "${RED}[✗] Compilation failed!${NC}"
    exit 1
fi

# Run the program
echo ""
echo "=================================================="
echo "Setup complete! The system is ready to run."
echo "Commands:"
echo "  make run       - Run the banking system"
echo "  make clean     - Remove compiled files"
echo "  make distclean - Remove all generated files"
echo "=================================================="
echo ""
echo "Start the program now? (y/n)"
read -r response

if [[ "$response" == "y" || "$response" == "Y" ]]; then
    make run
fi
