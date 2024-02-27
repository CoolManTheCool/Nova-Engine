#!/bin/bash
clear
# Create build directory if it doesn't exist
mkdir -p build

# Move into the build directory
cd build || exit

# Run cmake to generate build files
cmake ..

# Build the project using make
make

# Run the project using this little bro
./Dynamic-Voxel-Craft