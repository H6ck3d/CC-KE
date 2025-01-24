cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CC-KE)

# Add the executable
add_executable(CC-KE src/CC-KE.cpp lib/tinyxml2.cpp)

# Include directories
include_directories(lib)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Platform-specific settings
if (WIN32)
    # Windows specific code
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (UNIX)
    # Linux specific code
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()