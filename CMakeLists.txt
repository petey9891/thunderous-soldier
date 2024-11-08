cmake_minimum_required(VERSION 3.10)
project(thunderous-soldier)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory
include_directories(include)

# Set default build type to Release if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode")

# Compiler flags for different build types
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create the main executable
add_executable(main ${SOURCES})
target_link_libraries(main pthread)

# Add tests
enable_testing()
add_subdirectory(tests)
