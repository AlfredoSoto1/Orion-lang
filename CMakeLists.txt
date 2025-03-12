cmake_minimum_required(VERSION 3.22)

project(rootproject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add preprocessor definitions based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DUR_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DUR_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_definitions(-DUR_DIST)
endif()

# Add the subdirectories for each project
add_subdirectory(backend)
add_subdirectory(frontend)