add_library(assets STATIC
    mesh_data.cpp
    color_texture.cpp
    checker_texture.cpp
)

target_include_directories(assets PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(assets PUBLIC
    math
)