#!/bin/bash

####################################################################################
# Nova Studios Build Configuration
#
# Copyright (c) 2024, Noah Grimes (Nova Studios)
# All rights reserved.
# This file is part of the Nova Studios project,
# Therfore this file is licensed under the Nova Studios General Software License.
# 
# Usage:
# - TODO
#
####################################################################################

#!/bin/bash

case "$1" in
	-install)
        # Handle installation
        # ./run.sh -install
		echo "[ OK ] Installing dependencies."
        sudo apt install -y git cmake build-essential docker.io docker-compose
        docker-compose build
        # TODO
        ;;
    -help)
        # Handle help option
        # ./run.sh -help
        ;;
    -release)
        # Build Project in Docker Container, designed for Production binaries.
        docker-compose up
        ;;
    -compile)
        ./build.sh -release -norun
        ;;
    *)
       # Handle default case, build on this system.
        echo "[ OK ] Compiling."
        rm -rf build
        if [ -d "bin" ]; then
            # Remove all contents of the 'bin' directory
            rm -rf bin/*
        fi
		cmake app -B build -DCMAKE_BUILD_TYPE=Debug
        cmake --build build
esac
