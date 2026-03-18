# Using valgrind for memory check
find_program(VALGRIND_EXECUTABLE valgrind)

# Add subfolders
add_subdirectory(math_test)
add_subdirectory(modeling_test)
add_subdirectory(raytracer_test)

