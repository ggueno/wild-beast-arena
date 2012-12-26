#!/bin/bash
mkdir -p bin
echo "Compiling wba, this will take some time"
g++ src/*.cpp src/physics/*.cpp lib/stein/*.cpp lib/glew/*.c -I.  -lGL -lrt -lSDL -O3 -o bin/wba
echo "done ! Now go in the bin directory and launch ./wba"
