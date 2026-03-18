cmake_minimum_required(VERSION 3.23)

project(3D_lantern LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()  
add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)

