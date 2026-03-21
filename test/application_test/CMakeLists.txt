add_executable(application_test application_test.cpp)

target_link_libraries(application_test PUBLIC
        modeling
        application
)
