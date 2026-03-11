cmake_minimum_required(VERSION 3.23)

project(3D_lantern LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(test)

#add_executable()

#target_sources(PRIVATE )

#target_include_directories( PUBLIC math)

#target_compile_options(testkir PRIVATE -Wall)

