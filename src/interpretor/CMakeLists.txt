add_library(interpretor STATIC 
)

target_include_directories(interpretor PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(interpretor PUBLIC   
)