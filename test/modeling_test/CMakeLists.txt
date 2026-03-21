find_package(SDL2 REQUIRED)

file(GLOB TEST_SOURCES "*.cpp")

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_include_directories(${test_name} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${test_name} PRIVATE modeling glad ${SDL2_LIBRARIES} math)
    add_test(NAME ${test_name} COMMAND ${test_name})

    # Register Valgrind-wrapped test if Valgrind is available
    if(VALGRIND_EXECUTABLE)
    add_test(NAME ${test_name}_valgrind
             COMMAND ${VALGRIND_EXECUTABLE}
                     --leak-check=full
                     --track-origins=yes
                     --error-exitcode=1
                     --undef-value-errors=no
                     --partial-loads-ok=yes
                     $<TARGET_FILE:${test_name}>)
    endif()
endforeach()
