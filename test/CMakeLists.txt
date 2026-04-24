set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}bin/test)

# Using valgrind for memory check
find_program(VALGRIND_EXECUTABLE valgrind)

# Add subfolders
add_subdirectory(math_test)
add_subdirectory(modeling_test)
add_subdirectory(raytracer_test)
add_subdirectory(application_test)
add_subdirectory(assets_test)
add_subdirectory(interpreter_test)
