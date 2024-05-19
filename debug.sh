#!/bin/bash

if [ -f .env ]; then
  source .env
else
  echo ".env file not found!"
  exit 1
fi

cd $IDIR
gdb -batch -ex "file Nova-Engine" -ex "run" -ex "bt" -ex "exit"
