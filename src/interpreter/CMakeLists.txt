add_library(interpreter STATIC 
    lexer.cpp
    parser.cpp
)

target_include_directories(interpreter PUBLIC ${CMAKE_SOURCE_DIR}/src)
