# GLAD
add_library(glad STATIC glad/src/glad.c)
set_source_files_properties(glad/src/glad.c PROPERTIES LANGUAGE C)
target_include_directories(glad PUBLIC glad/include)

if(APPLE)
    target_link_libraries(glad PUBLIC "-framework OpenGL")
elseif(WIN32)
    target_link_libraries(glad PUBLIC opengl32)
else()
    target_link_libraries(glad PUBLIC GL)
endif()

# STB
add_library(stb_image STATIC stb/stb_image.cpp)
target_include_directories(stb_image PUBLIC stb)

# TINYOBJLOADER
add_library(tinyobjloader STATIC tinyObjLoader/tiny_obj_loader.cpp)
target_include_directories(tinyobjloader PUBLIC tinyObjLoader)