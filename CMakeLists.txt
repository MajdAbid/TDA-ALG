# Set minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MyProject)

# Set the output directory for the binary and object files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add executable for read_filtration.cpp
add_executable(read_filtration src/read_filtration.cpp)

