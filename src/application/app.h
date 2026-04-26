#ifndef APP_H
#define APP_H

#include "Window.h"
#include "gui.h"
#include "controller.h"
#include "modeling/MD_Renderer.h"
#include "modeling/MD_Scene.h"
#include "assets/shared_resources.h"
#include "modeling/modeling_resources.h"
#include "ray_tracer/raytracer_resources.h"

class App {
public:
    App(const std::string& title, int x, int y);
    ~App();

    void run();

private:
    // resources (constructed first, destroyed last
    SharedResources shared;
    ModelingResources modeling;
    RaytracerResources raytracer;

    // core systems
    Window window;
    MD_Renderer* renderer = nullptr;
    MD_Scene* scene = nullptr;
    GUI* gui = nullptr;

    Controller controller;

    
};

#endif