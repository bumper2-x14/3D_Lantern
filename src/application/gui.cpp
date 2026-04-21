#include "gui.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"


GUI::GUI(SDL_Window* window, SDL_GLContext gl_context)
    : window(window), gl_context(gl_context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");
}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUI::processEvent(const SDL_Event& e) {
    ImGui_ImplSDL2_ProcessEvent(&e);
}

void GUI::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void GUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::drawPanels(MD_Scene& scene, int width, int height, int panel_top, int panel_bottom,
                     int panel_left, int panel_right) {

    int viewport_h = height - panel_top - panel_bottom;
    int viewport_w = width  - panel_left - panel_right;

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    // TOP
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, panel_top));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("TopBar", nullptr, flags);
    ImGui::Text("Toolbar");
    ImGui::End();
    ImGui::PopStyleVar();

    // BOTTOM
    ImGui::SetNextWindowPos(ImVec2(0, height - panel_bottom));
    ImGui::SetNextWindowSize(ImVec2(width, panel_bottom));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("Status", nullptr, flags);
    ImGui::Text("Status bar");
    ImGui::End();
    ImGui::PopStyleVar();

    // LEFT
    ImGui::SetNextWindowPos(ImVec2(0, panel_top));
    ImGui::SetNextWindowSize(ImVec2(panel_left, viewport_h));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("Outliner", nullptr, flags);
    ImGui::Text("Scene objects");
    ImGui::End();
    ImGui::PopStyleVar();

    // RIGHT
    ImGui::SetNextWindowPos(ImVec2(width - panel_right, panel_top));
    ImGui::SetNextWindowSize(ImVec2(panel_right, viewport_h));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("Inspector", nullptr, flags);
    ImGui::Separator();

    if (ImGui::BeginTabBar("WORLD")) {
        // OBJECTS TAB
        if (ImGui::BeginTabItem("Objects")){
            ImGui::Separator();
            int i = 0;
            if (ImGui::BeginListBox("##objects_list", ImVec2(-1, 200))) {
                for (auto& obj : scene.objects) {
                    char label[100];
                    snprintf(label, sizeof(label), "Object : %s", obj->name.c_str());

                    bool is_selected = (scene.selected_obj_index == i);
                    if (ImGui::Selectable(label, is_selected))
                        scene.setSelectedObject(obj->selectable ? i : -1);
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    i++;
                }
                ImGui::EndListBox();
            }
            ImGui::EndTabItem();
        }

        // LIGHTS TAB
        if (ImGui::BeginTabItem("Lights"))
        {
            ImGui::Separator();

            int i = 0;
            if (ImGui::BeginListBox("##lights_list", ImVec2(-1, 200))) {
                for (auto& light : scene.point_lights) {
                    char label[100];
                    snprintf(label, sizeof(label), "Light : %d", i);

                    bool is_selected = (scene.selected_light_index == i);
                    if (ImGui::Selectable(label, is_selected))
                        scene.setSelectedPointLight(i);
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    i++;
                }
                ImGui::EndListBox();
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::Separator();
    ImGui::Text("Factory");

    ImGui::End();
    ImGui::PopStyleVar();
}