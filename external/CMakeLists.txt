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

# IMGUI
find_package(SDL2 REQUIRED)

add_library(imgui STATIC
    ImGui/imgui.cpp
    ImGui/imgui_draw.cpp
    ImGui/imgui_tables.cpp
    ImGui/imgui_widgets.cpp

    ImGui/backends/imgui_impl_sdl2.cpp
    ImGui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ImGui
    ImGui/backends
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(imgui PUBLIC glad SDL2 SDL2::SDL2)