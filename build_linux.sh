#!/bin/bash

# Check if the build directory exists
if [ ! -d "build" ]; then
    # Create the build directory if it doesn't exist
    mkdir build
fi

# Navigate to the build directory
cd build

# Run cmake and make to build the project
cmake ..
make