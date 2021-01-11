# Cmake version 3.0 or newer required
cmake_minimum_required(VERSION 3.0)

# Create compile command json file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(RaytracingInWeek
    VERSION 1.0
    LANGUAGES CXX
    DESCRIPTION "L-System implementation for C++ project")

# Include source/raytracedEngine
add_subdirectory(source/raytracedEngine)

# Include sandbox
# add_subdirectory(sandbox)

