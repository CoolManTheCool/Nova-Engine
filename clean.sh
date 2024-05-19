#!/bin/bash

sudo echo "User authenticated!"

if [ -f .env ]; then
  source .env
else
  echo ".env file not found!"
  exit 1
fi

rm -r build
# rm Nova-Engine
rm -f ./src/resources/shaders/fragment.frag.spv
rm -f ./src/resources/shaders/vertex.vert.spv

echo "Deleting $IDIR"
if [ -d "$IDIR" ]; then
    # If it exists, delete it
    sudo rm -r "$IDIR"
    echo "Directory $IDIR deleted."
fi