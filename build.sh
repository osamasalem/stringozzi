mkdir build_Linux 
cd build_Linux
cmake -Dgtest_force_shared_crt=TRUE  ..
cmake --build .

