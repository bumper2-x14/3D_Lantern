#ifndef WINDOW_H
#define WINDOW_H

#ifdef __APPLE__
  #define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>



class Window {
public:
    Window(const std::string& name = "window",
           int width  = 1280,
           int height = 720,
           int x      = SDL_WINDOWPOS_CENTERED,
           int y      = SDL_WINDOWPOS_CENTERED);
    ~Window();

    void winInitGl();
    void winRun();

    SDL_Window* getWin() const { return win; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getViewportW() const { return viewport_w; }
    int getViewportH() const { return viewport_h; }

private:
    void sdlSetAttributes();
    void updateViewport();          // recomputes viewport_w/h and calls glViewport

    SDL_Window* win = nullptr;
    SDL_GLContext gl_context = nullptr;

    int  width;
    int  height;

    // region reserved for the GL scene — the rest is ImGui panels
    int  viewport_w;                // set in winRun / updated on resize
    int  viewport_h;
    int  panel_width  = 300;        // right-side ImGui panel width
    int  panel_height = 0;          // bottom panel height (0 = none yet)

    bool stop = false;
};





#endif

