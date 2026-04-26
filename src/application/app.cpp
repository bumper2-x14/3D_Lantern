#include "app.h"

App::App(const std::string& title, int x, int y)
    : window(title, x, y)
{
    // create GL context first
    window.winInitGl();

    // now we can init GUI — it needs a valid GL context
    int lw, lh;
    SDL_GetWindowSize(window.getWin(), &lw, &lh);
    renderer = new MD_Renderer();
    scene = new MD_Scene();

    gui = new GUI(window.getWin(), window.getGLContext(),
              lw, lh,
              window.getPanelBottom(), window.getPanelTop(),
              window.getPanelLeft(),   window.getPanelRight());
}

App::~App() {
    delete renderer;
    delete scene;
    delete gui;
}

void App::run() {
    window.winRun(*gui, controller, *scene, *renderer, shared, modeling);
}