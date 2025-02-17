# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(event_processor)

# Set the output directory to the first-level "build" directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find all .cpp files in the current directory and subdirectories
file(GLOB_RECURSE SOURCES "*.cpp")

# Exclude CMake's internal compiler test files
list(FILTER SOURCES EXCLUDE REGEX "CMakeCXXCompilerId.cpp")

# Check if any source files are found
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found.")
endif()

# Create the executable
add_executable(event_processor ${SOURCES})

# Set the C++ standard to C++20 to support std::construct_at
set_target_properties(event_processor PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)

# Optionally print the C++ standard to verify it's set
message(STATUS "Using C++ standard: C++20")
