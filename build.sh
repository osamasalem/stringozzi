#!/bin/bash
set -e

mkdir -p build_Linux 
cd build_Linux
cmake -DCMAKE_BUILD_TYPE=Release -Dgtest_force_shared_crt=TRUE  ..
cmake --build .

../bin_Linux/stringozzi.test
