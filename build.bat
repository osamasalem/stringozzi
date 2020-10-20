@echo off
mkdir build_Windows
cd build_Windows
cmake -Dgtest_force_shared_crt=TRUE  .. 
cmake.exe --build . 
cd ..

