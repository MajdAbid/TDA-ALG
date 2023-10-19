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
add_executable(barcode_filtration "${CMAKE_CURRENT_SOURCE_DIR}/src/barcode_filtration.cpp")



# Add header files to the target
target_include_directories(read_filtration PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(barcode_filtration PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
# Link the executable with any required libraries
#target_link_libraries(my_binary my_library)
target_link_libraries(barcode_filtration "${CMAKE_CURRENT_SOURCE_DIR}/src/read_filtration.cpp")

# Install the binary to the "bin" directory
install(TARGETS read_filtration DESTINATION bin)
install(TARGETS barcode_filtration DESTINATION bin)

# Pass the data file path to the executable as an argument
target_compile_definitions(read_filtration PRIVATE READ_FILTRATION_MAIN)
target_compile_definitions(barcode_filtration PRIVATE BARCODE_FILTRATION_MAIN)

