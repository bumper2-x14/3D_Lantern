add_library(modeling STATIC
    MD_Mesh.cpp
    MD_Camera.cpp
    MD_Shader.cpp
    MD_Object.cpp
    MD_Sphere.cpp
)


target_include_directories(modeling PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(modeling PUBLIC
    math
    assets
    glad
)
