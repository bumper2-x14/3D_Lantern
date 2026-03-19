add_library(modeling STATIC
    MD_Mesh.cpp
    MD_Camera.cpp
    MD_Shader.cpp
)


target_include_directories(modeling PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(modeling PUBLIC
    math
    assets
    glad
)
