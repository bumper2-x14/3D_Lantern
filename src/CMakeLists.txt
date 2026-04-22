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
target_link_libraries(lantern PRIVATE
    interpreter
    ray_tracer
    assets
    math
)