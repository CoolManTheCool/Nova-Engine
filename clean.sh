#!/bin/bash

sudo echo "User authenticated!"

if [ -f .env ]; then
  source .env
else
  echo ".env file not found!"
  exit 1
fi
echo "Deleting build & configuration"
rm -r build
# rm Nova-Engine
echo "Deleting compiled shaders!"
rm -f ./src/resources/shaders/fragment.frag.spv
rm -f ./src/resources/shaders/vertex.vert.spv


if [ -d "$IDIR" ]; then
    # If it exists, delete it
    sudo rm -r "$IDIR"
    echo "Directory $IDIR deleted."
fi