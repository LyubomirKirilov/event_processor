# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(event_processor)

# Explicitly set GCC as the compiler (optional, remove if using default system compiler)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++-13)

# Check for C++20 support in the selected compiler
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++20" COMPILER_SUPPORTS_CXX20)

if(NOT COMPILER_SUPPORTS_CXX20)
    message(FATAL_ERROR "Compiler does not support C++20. Please use GCC 10+ or Clang 10+.")
endif()

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

# Set the C++ standard to C++20 to support std::ranges
set_target_properties(event_processor PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)

# Optionally print the C++ standard and compiler used
message(STATUS "Using C++ standard: C++20")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
