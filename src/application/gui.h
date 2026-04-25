#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "imgui.h"
#include "controller.h"

#include "modeling/MD_Camera.h"
#include "modeling/MD_Scene.h"

class GUI {
public:
    GUI(SDL_Window* window, SDL_GLContext gl_context, int height,
                int width, int panel_bottom, int panel_top,
                int panel_left, int panel_right);
    ~GUI();

    void begin();   // start ImGui frame
    void render();  // render ImGui

    void processEvent(const SDL_Event& e);

    void drawPanels(MD_Scene& scene, int width, int height, int panel_top,int panel_bottom,
                        int panel_left, int panel_right);

    void drawPanelTop(MD_Scene& scene);

    void drawPanelBottom(MD_Scene& scene, MD_Camera& camera);

    void drawPanelRight(MD_Scene& scene);

    void drawPanelLeft(MD_Scene& scene);

    void setSelectedTool(CtrlMode mode) { selected_tool = mode; }
    CtrlMode getSelectedTool() const { return selected_tool; }

private:
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;
    int width = 0;
    int height = 0;
    int panel_top = 0;
    int panel_bottom = 0;
    int panel_right = 0;
    int panel_left = 0;

    CtrlMode selected_tool = CtrlMode::CAMERA;

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    GLuint icon_sphere;
    GLuint icon_cylinder;
    GLuint icon_cone;
    GLuint icon_torus;    
};

#endif
