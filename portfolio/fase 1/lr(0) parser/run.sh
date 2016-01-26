#!/bin/bash

echo "Making and building the program."

#Creates the bin folder(where the executables are located)
mkdir bin

#Build of the exec
mkdir build
cd build
cmake ..
make
make install

echo "The input CFG file"
cat ../input_CFG.xml

#Running the exec
../bin/exec ../input_CFG.xml
