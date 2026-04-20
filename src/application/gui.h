#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

class GUI {
public:
    GUI(SDL_Window* window, SDL_GLContext gl_context);
    ~GUI();

    void begin();   // start ImGui frame
    void render();  // render ImGui

    void processEvent(const SDL_Event& e);

    void drawPanels(int width, int height, int panel_top,int panel_bottom,
                        int panel_left, int panel_right);

private:
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;
};

#endif