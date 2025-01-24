#!/bin/bash

# Build directory and destination file
DEST_DIR=~/programing/Cpp/statusp/bin
DEST_FILE="$DEST_DIR/statusp"

# Ensure the destination directory exists
if [ ! -d "$DEST_DIR" ]; then
    echo "Destination directory $DEST_DIR does not exist. Creating it..."
    mkdir -p "$DEST_DIR"
fi

# Compilation with g++
echo "Compiling the project..."
g++ -std=c++17 -Wall -Wextra -o "$DEST_FILE" ./src/status_main.cpp
if [ $? -ne 0 ]; then
    echo "Compilation failed. Aborting."
    exit 1
fi

# Give execution permissions to the binary
chmod +x "$DEST_FILE"
echo "Execution permissions granted to $DEST_FILE."

echo "Compilation completed successfully."
