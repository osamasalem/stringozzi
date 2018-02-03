mkdir build & cd build 
cmake -DENABLE_COVERAGE=TRUE -Dgtest_force_shared_crt=TRUE  ..
cmake --build 

