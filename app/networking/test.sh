#!/bin/bash

# Compile the source files
g++ -std=c++23 -o network main.cpp client.cpp server.cpp -lboost_system -lpthread -g -O2 -Wall
