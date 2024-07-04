#!/bin/bash

#tTo remove the build directory
rm -rf build

#To build the build directory
cmake -Bbuild

#move into build directory
cd build

#create the make file 
make 

