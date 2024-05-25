#!/bin/bash

sudo echo "[ OK ] User authenticated!"

ENV_FILE=".env"
WD=$PWD
if [ -f $ENV_FILE ]; then
  source $ENV_FILE
else
echo " [ BAD ].env file not found! Creating default environment variables."
  
cat > $ENV_FILE <<EOF
IDIR=/media/noah/writable/Nova-Engine/
EOF
echo " [ OK ] Done creating default environment variables, please rerun this script! Terminating"
exit 1
fi

# Check if the "-clean" flag is provided
if [ "$1" = "-clean" ]; then
    # If the "-clean" flag is provided, clean up and exit
    echo "[ OK ] Cleaning up"
    # Add any cleanup commands here
    ./clean.sh
fi

# Create build directory if it doesn't exist
mkdir -p build

# Move into the build directory
cd build || exit

# Run cmake to generate build files
cmake ..

# /media/noah/writable/Nova-Engine/

install() {
    echo "[ OK ] Install Directory: $IDIR (Change this in your .env)"
    ./resources.sh

    sudo cp build/Nova-Engine $IDIR/Nova-Engine
    cd $IDIR

    echo "[ OK ] Nova-Engine and resources copied to project install directory."
}

# Build the project using make
if make; then
    # If compilation succeeds, check if the executable exists
    if [ -f "Nova-Engine" ]; then
        # If the executable exists, copy it to the project root directory
        echo "[ OK ] Compilation succeeded."
        cd ..
        install
        cd $WD
        #sleep 0.5
    else
        # If the executable doesn't exist, print an error message
        echo "[ FAILURE ] Error: Executable not found."
        echo "[ FAILURE ]Compilation Failed."
        exit 1
    fi
else
    # If compilation fails, print an error message
    echo "[ FAILURE ] Compilation Failed."
    exit 1
fi

cd $WD
./run.sh