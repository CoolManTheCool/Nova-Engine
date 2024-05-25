#!/bin/bash

# Authenticate the user
sudo echo "[ OK ] User authenticated!"

# Source the .env file if it exists
if [ -f .env ]; then
  source .env
else
  echo "[ FAILURE ] .env file not found!"
  exit 1
fi

# Remove existing compiled shaders

echo "[ OK ] Compiling shaders"

sudo rf -r $IDIR

mkdir ${IDIR}
mkdir ${IDIR}resources
mkdir ${IDIR}resources/shaders

# Find all .vert and .frag shader files
shaders=$(find ./src/resources/shaders -type f \( -name "*.vert" -o -name "*.frag" \))

# Process each shader file
for shader in $shaders; do
    if [[ $shader == *.vert ]]; then
        output="${shader%.vert}.spv"
    elif [[ $shader == *.frag ]]; then
        output="${shader%.frag}.spv"
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

echo "[ OK ] All shaders compiled successfully."

echo "[ OK ] Copying resources into $IDIR"

# Copy resources to the specified directory
sudo cp -r $PWD/src/resources ${IDIR}
rm ./src/resources/shaders/*.spv