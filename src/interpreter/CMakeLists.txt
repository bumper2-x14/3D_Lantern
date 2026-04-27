add_library(interpreter STATIC 
    lexer.cpp
    parser.cpp
    interpreter.cpp
    scene_serializer.cpp
)

target_include_directories(interpreter PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(interpreter PUBLIC modeling)
