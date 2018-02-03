@echo off
mkdir build
cd build
cmake -Dgtest_force_shared_crt=TRUE  .. 
cmake.exe --build . 

