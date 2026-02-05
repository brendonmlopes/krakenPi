#!/bin/bash

# Enhanced Arduino compile and upload script with error handling

echo "Checking for Arduino boards..."

# Check if arduino-cli is available
if ! command -v arduino-cli &> /dev/null; then
    echo "Error: arduino-cli not found. Please install arduino-cli first."
    exit 1
fi

# Check for connected Arduino boards
BOARD_LIST=$(arduino-cli board list 2>/dev/null)
if [[ -z "$BOARD_LIST" || "$BOARD_LIST" == *"No boards found"* ]]; then
    echo "Error: No Arduino boards found. Please check:"
    echo "  - Arduino is connected via USB"
    echo "  - USB cable is properly seated"
    echo "  - Arduino drivers are installed (if needed)"
    exit 1
fi

echo "Available boards:"
echo "$BOARD_LIST"
echo ""

# Extract first available serial port (skip header line)
PORT=$(echo "$BOARD_LIST" | grep "/dev/tty" | head -1 | awk '{print $1}')

if [[ -z "$PORT" ]]; then
    echo "Error: Could not determine Arduino port"
    echo "Available serial devices:"
    ls -la /dev/tty* | grep "tty" | head -5
    exit 1
fi

# Check if the port is actually accessible
if [[ ! -c "$PORT" ]]; then
    echo "Error: Port $PORT is not a character device"
    exit 1
fi

echo "Using Arduino on port: $PORT"

# Warn if this might not be an actual Arduino
if echo "$BOARD_LIST" | grep "$PORT" | grep -q "Unknown"; then
    echo "⚠️  Warning: $PORT shows as 'Unknown' - this may not be an Arduino"
    echo "   Continuing anyway, but upload may fail..."
    echo ""
fi

# Check if .ino file exists
if [[ ! -f "peripheral.ino" ]]; then
    echo "Error: peripheral.ino not found in current directory"
    exit 1
fi

echo "Compiling and uploading..."
echo "Board: Arduino Uno"
echo "Port: $PORT"
echo "Sketch: peripheral.ino"
echo ""

# Compile and upload (remove sudo since user is in uucp group)
if arduino-cli compile -b arduino:avr:uno --upload -p "$PORT" .; then
    echo ""
    echo "✅ Success! Arduino sketch compiled and uploaded successfully."
else
    echo ""
    echo "❌ Failed to compile or upload sketch."
    echo "Please check the error messages above."
    exit 1
fi
