file(GLOB_RECURSE TEST_SOURCES "*.cpp")

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE math ray_tracer)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
