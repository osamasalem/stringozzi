#!/bin/bash
set -e

export CXXFLAGS=-m32
mkdir -p build_Linux_DEBUG_x86 
cd build_Linux_DEBUG_x86
cmake -DCMAKE_BUILD_TYPE=Debug -DARCH=x86 -Dgtest_force_shared_crt=TRUE -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
cmake --build . --config Debug
cd ..


export CXXFLAGS=-m32
mkdir -p build_Linux_RELEASE_x86 
cd build_Linux_RELEASE_x86
cmake -DCMAKE_BUILD_TYPE=Release -DARCH=x86 -Dgtest_force_shared_crt=TRUE -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
cmake --build . --config Release
cd ..

export CXXFLAGS=-m64
mkdir -p build_Linux_DEBUG_x64 
cd build_Linux_DEBUG_x64
cmake -DCMAKE_BUILD_TYPE=Debug -DARCH=x64 -Dgtest_force_shared_crt=TRUE -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
cmake --build . --config Debug
cd ..

export CXXFLAGS=-m64
mkdir -p build_Linux_RELEASE_x64 
cd build_Linux_RELEASE_x64
cmake -DCMAKE_BUILD_TYPE=Release -DARCH=x64 -Dgtest_force_shared_crt=TRUE -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
cmake --build . --config Release
cd ..


bin.tmp/Linux/Debug/x64/stringozzi.test