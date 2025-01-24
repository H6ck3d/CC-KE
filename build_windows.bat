@echo off

REM Check if the build directory exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

REM Change to the build directory
cd build

REM Run CMake and Make commands for Windows
cmake .. -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM="C:/Program Files (x86)/GnuWin32/bin/make.exe" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
make