@echo off
mkdir build_Windows
cd build_Windows
cmake -DCMAKE_BUILD_TYPE=Release -Dgtest_force_shared_crt=TRUE  .. 
cmake.exe --build . 
cd ..

