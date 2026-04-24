cmake_minimum_required(VERSION 3.23)

project(3D_lantern LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(
    EXAMPLE_OUTPUT_DIR="${CMAKE_SOURCE_DIR}/example/"
    IMG_DIR="${CMAKE_SOURCE_DIR}/img/"
    RESOURCE_DIR="${CMAKE_SOURCE_DIR}/resources/"
    SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/"
    ROOT_DIR="${CMAKE_SOURCE_DIR}/"
)



# Enabled for code execution and memory testing
enable_testing()  

add_subdirectory(external)
add_subdirectory(test)
add_subdirectory(example)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(src)


