@echo off
nuget restore -PackagesDirectory packages
md build
cd build
..\packages\CMake.3.5.2\bin\cmake.exe ..
..\packages\CMake.3.5.2\bin\cmake.exe --build . --config Debug
..\packages\CMake.3.5.2\bin\cmake.exe --build . --config Release

cd ..
