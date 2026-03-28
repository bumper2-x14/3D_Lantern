file(GLOB EXAMPLE_SOURCES "*.cpp")

foreach(EXAMPLE_SRC ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SRC} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SRC})
    target_compile_definitions(${EXAMPLE_NAME} PRIVATE
        EXAMPLE_OUTPUT_DIR="${CMAKE_SOURCE_DIR}/example/"
    )

    target_compile_definitions(${EXAMPLE_NAME} PRIVATE
        IMG_DIR="${CMAKE_SOURCE_DIR}/img/"
    )
    target_link_libraries(${EXAMPLE_NAME} PRIVATE ray_tracer math)
endforeach()