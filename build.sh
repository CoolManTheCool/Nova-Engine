#!/bin/bash

####################################################################################
# Nova Studios Build Configuration
#
# Copyright (c) 2024, Noah Grimes (Nova Studios)
# All rights reserved.
# This file is part of a Nova Studios project,
# Therfore this file is licensed under the Nova Studios General Software License.
# 
# 
# Usage:
# - Create a CMakeLists.txt file.
# - Change PROJECT_NAME to match the project name in the CMakeLists.txt
# - Add this file to your project directory.
# - Run the script to configure, build, and manage resources.
# - Run the script with the "-clean" flag to clean up and exit. (Never required.)
#
####################################################################################

### CONFIGURATION ###
# Project Name (Executable name, shared with CMakeLists.txt)
PROJECT_NAME="Nova-Engine"

### END CONFIGURATION ###

sudo echo "[ OK ] User authenticated!"

if [ "$1" = "-help" ]; then
  echo "Usage: $0 [options]"
  echo "Options:"
  echo " -help        : Print this help message"
  echo " -clean       : Clean up and exit"
  echo " -debug       : Enable debug output"
  echo " -release     : (WIP) Build a release version"
  exit 0
fi

### Check if the "-clean" flag is provided
if [ "$1" = "-clean" ]; then
  # If the "-clean" flag is provided, clean up and exit
  echo "[ OK ] Cleaning up"
  # Add any cleanup commands here
  echo "[ OK ] Deleting build & configuration"
  rm -rf ./build
  rm -rf ./bin   
  exit
fi

### Configure the project
echo "[ OK ] Configuring project..."
if [ "$2" = "-release" ] || [ "$1" = "-release" ]; then
  cmake app -B build -DCMAKE_BUILD_TYPE=Release
else
	cmake app -B build -DCMAKE_BUILD_TYPE=Debug
fi
if [ $? -ne 0 ]; then
  echo "[ FAIL ] CMake configuration failed!"
  exit 1
fi

### Build the project
echo "[ OK ] Building project..."
cmake --build build
if [ $? -ne 0 ]; then
  echo "[ FAIL ] Build failed!"
  exit 1
fi

# Cache Resources
cp -r ./app/resources ./build/resources

### Compile Shaders
echo "[ OK ] Compiling shaders..."
# Find all .vert and .frag shader files
shaders=$(find ./build/resources/shaders -type f \( -name "*.vert" -o -name "*.frag" \))

# Process each shader file
for shader in $shaders; do
    if [[ $shader == *.vert ]]; then
        output="$shader.spv"
    elif [[ $shader == *.frag ]]; then
        output="$shader.spv"
    else
        echo "[ UNKNOWN ] Unknown shader type: $shader"
        exit 1
    fi
    
    if glslc "$shader" -o "$output"; then
        echo "[ OK ] Compiled $shader to $output"
    else
        echo "[ FAILURE ] Shader compilation failed for $shader"
        exit 1
    fi
done

mkdir bin/resources
cp -r build/resources bin/
echo "[ OK ] $PROJECT_NAME copied to ./bin."
# Delete build files.
# rm -rf build

echo "[ OK ] Nova Studio Build Configuration Completed."
echo "[ OK ] Build finished without errors."

### Run the project

if [ "$1" != "-norun" ] || [ "$2" = "-norun"]; then
  echo
  echo
  echo
  if [ "$1" = "-debug" ]; then
    gdb -batch -ex "file ./bin/$PROJECT_NAME" -ex "run" -ex "bt" -ex "exit"
  else
    ./bin/$PROJECT_NAME
fi
fi