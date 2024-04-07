#!/bin/bash

# Function to clean up
clean_up() {
    echo "Cleaning up"
    # Add any cleanup commands here
    ./clean.sh
}

# Check if the "-clean" flag is provided
if [ "$1" = "-clean" ]; then
    # If the "-clean" flag is provided, clean up and exit
    clean_up
fi

# If the "-clean" flag is not provided, continue with building

# Remove existing shader files
if [ ! -f "./src/resources/shaders/fragment.frag.spv" ] || [ ! -f "./src/resources/shaders/vertex.vert.spv" ]; then
    # Compile shaders only if they don't exist
    echo "Compiling shaders"
    if glslc ./src/resources/shaders/fragment.frag -o ./src/resources/shaders/fragment.frag.spv && \
       glslc ./src/resources/shaders/vertex.vert -o ./src/resources/shaders/vertex.vert.spv; then
        echo "Shader compilation succeeded."
    else
        echo "Shader compilation failed."
        exit 1
    fi
fi

# Create build directory if it doesn't exist
mkdir -p build

# Move into the build directory
cd build || exit

# Run cmake to generate build files
cmake ..

# Build the project using make
if make; then
    # If compilation succeeds, check if the executable exists
    if [ -f "Nova-Engine" ]; then
        # If the executable exists, copy it to the project root directory
        echo "Compilation succeeded."
        cp Nova-Engine ..
        echo "Nova-Engine copied to project root directory."
        ./Nova-Engine
    else
        # If the executable doesn't exist, print an error message
        echo "Error: Executable not found."
        echo "Compilation Failed."
        exit 1
    fi
else
    # If compilation fails, print an error message
    echo "Compilation Failed."
    exit 1
fi
