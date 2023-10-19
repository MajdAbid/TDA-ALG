# Set minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(TDA-algorithms)

# Set the output directory for the binary and object files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the "src" directory for source code
#add_subdirectory(src)

# Add the "resources" directory for resource files
#add_subdirectory(resources)

# Add executable for main.cpp
add_executable(read_filtration "${CMAKE_CURRENT_SOURCE_DIR}/src/read_filtration.cpp")

# Link the executable with any required libraries
#target_link_libraries(my_binary my_library)

# Install the binary to the "bin" directory
install(TARGETS read_filtration DESTINATION bin)
# Set the default data file path
set(DEFAULT_DATA_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ressources/my_data_file.txt")

# Add a command line option to specify the data file path
option(DATA_FILE_PATH "Path to the data file" ${DEFAULT_DATA_FILE_PATH})

# Pass the data file path to the executable as an argument
target_compile_definitions(read_filtration PRIVATE DATA_FILE_PATH="${DATA_FILE_PATH}")
