#ifndef WINDOW_H
#define WINDOW_H

#include "modeling/MD_Renderer.h"
#include "assets/shared_resources.h"
#include "modeling/modeling_resources.h"

#ifdef __APPLE__
  #define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>
#include "gui.h"


class Window {
public:
    Window(const std::string& name = "window",
           int x = SDL_WINDOWPOS_CENTERED,
           int y = SDL_WINDOWPOS_CENTERED);

    ~Window();

    void winInitGl();
    void winRun(GUI& gui, Controller& controller,
                    MD_Scene& scene, MD_Renderer& renderer,
                    SharedResources& shared, ModelingResources& modeling);

    SDL_Window* getWin() const { return win; }
    SDL_GLContext getGLContext() const { return gl_context; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getViewportW() const { return viewport_w; }
    int getViewportH() const { return viewport_h; }
    int getPanelTop()    const { return panel_top; }
    int getPanelBottom() const { return panel_bottom; }
    int getPanelLeft()   const { return panel_left; }
    int getPanelRight()  const { return panel_right; }

private:
    void sdlSetAttributes();
    void updateViewport(); // recomputes viewport_w/h and calls glViewport

    SDL_Window* win = nullptr;
    SDL_GLContext gl_context = nullptr;

    int width = 0;
    int height = 0;
    int logical_w = 0, logical_h = 0;

    // region reserved for the GL scene — the rest is ImGui panels
    int viewport_w = 0; // set in winRun / updated on resize
    int viewport_h = 0;

    // Panels expressed as fractions of the display size
    static constexpr float kTopPanelFrac    = 0.1f; // ~4% of height  (toolbar)
    static constexpr float kBottomPanelFrac = 0.08f; // ~4% of height  (status bar)
    static constexpr float kLeftPanelFrac   = 0.07f; // ~12% of width  (outliner)
    static constexpr float kRightPanelFrac  = 0.20f; // ~20% of width  (properties)

    int panel_top = 0; // derived in constructor / updateViewport
    int panel_bottom = 0;
    int panel_left = 0;
    int panel_right = 0;

    GUI* gui = nullptr;
     
    bool stop = false;
};

#endif

