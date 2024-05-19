#!/bin/bash

sudo echo "[ OK ] User authenticated!"

if [ -f .env ]; then
  source .env
else
  echo "[ FAILURE ] .env file not found!"
  exit 1
fi

rm ./src/resources/shaders/*.spv

echo "[ OK ] Compiling shaders"

# Find and compile each shader file
find ./src/resources/shaders -type f \( -name "*.vert" -o -name "*.frag" \) | while read -r shader; do
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

sudo cp -r $PWD/src/resources $IDIR/resources