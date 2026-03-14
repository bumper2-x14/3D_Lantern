#include <cassert>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "modeling/MD_Camera.h"

int main()
{
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


    MD_Camera::regressionTest();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
