add_subdirectory(math)
add_subdirectory(assets)
add_subdirectory(modeling)
add_subdirectory(ray_tracer)
add_subdirectory(interpreter)
add_subdirectory(application)

# Lantern
add_executable(lantern lantern.cpp)
target_include_directories(lantern PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(lantern3D lantern3D.cpp)

target_link_libraries(lantern3D PRIVATE
    application
)

set_target_properties(lantern PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(lantern PRIVATE
    application
    modeling
    interpreter
    ray_tracer
    assets
    math
)
