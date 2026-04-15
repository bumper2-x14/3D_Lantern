add_library(modeling STATIC
    MD_Mesh.cpp
    MD_Camera.cpp
    MD_Shader.cpp
    MD_Object.cpp
    MD_Sphere.cpp
    MD_Quad.cpp
    MD_Cylinder.cpp
    MD_Circle.cpp
    MD_Cone.cpp
    MD_Torus.cpp
    MD_Renderer.cpp
    MD_Texture.cpp
    MD_Scene.cpp
    MD_Renderer.cpp
)


target_include_directories(modeling PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(modeling PUBLIC
    math
    assets
    glad
)
