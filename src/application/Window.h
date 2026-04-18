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
           int x = SDL_WINDOWPOS_CENTERED,
           int y = SDL_WINDOWPOS_CENTERED);

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
    void updateViewport(); // recomputes viewport_w/h and calls glViewport

    SDL_Window* win = nullptr;
    SDL_GLContext gl_context = nullptr;

    int width = 0;
    int height = 0;

    // region reserved for the GL scene — the rest is ImGui panels
    int viewport_w = 0; // set in winRun / updated on resize
    int viewport_h = 0;

    // Panels expressed as fractions of the display size
    static constexpr float kTopPanelFrac    = 0.1f; // ~4% of height  (toolbar)
    static constexpr float kBottomPanelFrac = 0.04f; // ~4% of height  (status bar)
    static constexpr float kLeftPanelFrac   = 0.05f; // ~12% of width  (outliner)
    static constexpr float kRightPanelFrac  = 0.20f; // ~20% of width  (properties)

    int panel_top = 0; // derived in constructor / updateViewport
    int panel_bottom = 0;
    int panel_left = 0;
    int panel_right = 0;
     
    bool stop = false;
};





#endif

