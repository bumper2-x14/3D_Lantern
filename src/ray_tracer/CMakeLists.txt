add_library(ray_tracer STATIC 
    RT_Sphere.cpp
    RT_Disk.cpp
    RT_Cylinder.cpp
    RT_Cone.cpp
    RT_Quad.cpp
    RT_Box.cpp
    RT_ObjectList.cpp
    RT_Camera.cpp
    RT_Renderer.cpp
    RT_Lambertian.cpp
    RT_Metallic.cpp
    RT_Dielectric.cpp
    RT_Isotropic.cpp
    RT_BVH.cpp
    RT_Medium.cpp
    RT_PointLight.cpp
    RT_DirectionalLight.cpp
)

target_include_directories(ray_tracer PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(ray_tracer PUBLIC
    math
    assets
)