add_library(interpreter STATIC 
    lexer.cpp
    parser.cpp
    interpreter.cpp
)

target_include_directories(interpreter PUBLIC ${CMAKE_SOURCE_DIR}/src)
