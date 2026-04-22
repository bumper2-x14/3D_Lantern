#include "modeling/MD_Object.h"
#include "modeling/MD_Sphere.h"
#include <glad/glad.h>
#include <SDL2/SDL.h>

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return -1;

    // Request OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create a hidden window
    SDL_Window* window = SDL_CreateWindow(
        "test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN
    );

    if (!window)
        return -1;

    // Create OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
        return -1;

    // Load OpenGL functions via GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        return -1;
    
    std::cout << "MD_Object regression test..." << std::endl;

    TRSDataf trs;
    trs.translation = Vec3f(1.0f, 2.0f, 3.0f);
    trs.rotation = Vec3f(10.0f, 20.0f, 30.0f);
    trs.scale = Vec3f(1.0f, 2.0f, 1.5f);

    MD_Sphere sphere(10, 10);
    MD_Object obj("test_object", &sphere, nullptr, trs);

    std::string result = obj.serialize();

    std::cout<<result<<std::endl;

    std::cout << "MD_Object serialize test passed." << std::endl;
} 