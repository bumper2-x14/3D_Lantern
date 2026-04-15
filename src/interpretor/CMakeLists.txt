add_library(interpretor STATIC 
    lexer.cpp
    parser.cpp
)

target_include_directories(interpretor PUBLIC ${CMAKE_SOURCE_DIR}/src)

#target_link_libraries(interpretor PUBLIC )