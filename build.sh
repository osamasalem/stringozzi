@echo off
mkdir googletest
git clone https://github.com/google/googletest.git googletest
mkdir build
cd build
cmake -Dgtest_force_shared_crt=TRUE  .. 
cmake.exe --build . 
cd ..
