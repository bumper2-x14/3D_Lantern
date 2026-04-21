add_library(application STATIC
    Window.cpp
    input.cpp
    controller.cpp
    gui.cpp
)

find_package(SDL2 REQUIRED)
target_include_directories(application PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(application PUBLIC  
    imgui
    glad
    ${SDL2_LIBRARIES}
    modeling
)

target_include_directories(application PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(application PUBLIC ${CMAKE_SOURCE_DIR}/src)



