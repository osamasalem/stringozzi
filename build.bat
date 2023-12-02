@echo off

md build_Windows_DEBUG_x86 
cd build_Windows_DEBUG_x86
cmake -A Win32 -DCMAKE_BUILD_TYPE=Debug -DARCH=x86 -Dgtest_force_shared_crt=TRUE -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
cmake --build . --config Debug
cd ..

md build_Windows_RELEASE_x86 
cd build_Windows_RELEASE_x86
cmake -A Win32 -DCMAKE_BUILD_TYPE=Release -DARCH=x86 -Dgtest_force_shared_crt=TRUE -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
cmake --build . --config Release
cd ..

md build_Windows_DEBUG_x64 
cd build_Windows_DEBUG_x64
cmake -A x64 -DCMAKE_BUILD_TYPE=Debug -DARCH=x64 -Dgtest_force_shared_crt=TRUE -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
cmake --build . --config Debug
cd ..

md build_Windows_RELEASE_x64 
cd build_Windows_RELEASE_x64
cmake -A x64 -DCMAKE_BUILD_TYPE=Release -DARCH=x64 -Dgtest_force_shared_crt=TRUE -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
cmake --build . --config Release
cd ..

doxygen docs\Doxyfile

xcopy docs\html dist\doc /E /I /y
xcopy include dist\include /E /I /y
xcopy src dist\src /E /I /y
copy README.md dist /y
copy LICENSE.md dist /y

bin.tmp\Windows\Debug\x86\stringozzi.test.exe
bin.tmp\Windows\Debug\x64\stringozzi.test.exe
bin.tmp\Windows\Release\x86\stringozzi.test.exe
bin.tmp\Windows\Release\x64\stringozzi.test.exe

