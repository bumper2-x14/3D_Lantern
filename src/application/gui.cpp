#include "gui.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"


GUI::GUI(SDL_Window* window, SDL_GLContext gl_context, 
                int _width, int _height, int _panel_bottom,
                int _panel_top, int _panel_left, int _panel_right)
    : window(window), gl_context(gl_context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale =1.3f;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");
 
    width = _width;
    height = _height; 
    panel_bottom = _panel_bottom;
    panel_top = _panel_top;
    panel_left = _panel_left;
    panel_right = _panel_right;
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

void GUI::drawPanelTop(MD_Scene& scene){
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, panel_top));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("TopBar", nullptr, flags);
    ImGui::Text("Toolbar");

    static int selected = 0;
    ImGui::RadioButton("move camera", &selected, 0); ImGui::SameLine();
    ImGui::RadioButton("move object", &selected, 1); ImGui::SameLine();
    ImGui::RadioButton("rotate object", &selected, 2);

    ImGui::End();
    ImGui::PopStyleVar();

   }

void GUI::drawPanelBottom(MD_Scene& scene){
    
    ImGui::SetNextWindowPos(ImVec2(0, height - panel_bottom));
    ImGui::SetNextWindowSize(ImVec2(width, panel_bottom));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("Status", nullptr, flags);
    ImGui::Text("Status bar");
    ImGui::End();
    ImGui::PopStyleVar();
    
}

void GUI::drawPanelLeft(MD_Scene& scene){
    
    ImGui::SetNextWindowPos(ImVec2(0, panel_top));
    ImGui::SetNextWindowSize(ImVec2(panel_left, height - panel_top - panel_bottom));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("Outliner", nullptr, flags);
    ImGui::Text("Scene objects");
    ImGui::End();
    ImGui::PopStyleVar();
    
}

void GUI::drawPanelRight(MD_Scene& scene){
    ImGui::SetNextWindowPos(ImVec2(width - panel_right, panel_top));
    ImGui::SetNextWindowSize(ImVec2(panel_right, height - panel_top - panel_bottom));
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
            ImGui::Separator();
            ImGui::Text("Add shape");

            if (ImGui::Button("Add Sphere")) {
                scene.createObject("Sphere", &scene.default_sphere,
                                TRSDataf{{0.f, 1.f, 0.f}},
                                &scene.default_sphere_mat);
            }

            if (ImGui::Button("Add Cylinder")) {
                scene.createObject("Cylinder", &scene.default_cylinder,
                                TRSDataf{{2.f, 1.f, 0.f}},
                                &scene.default_cylinder_mat);
            }

            if (ImGui::Button("Add Cone")) {
                scene.createObject("Cone", &scene.default_cone,
                                TRSDataf{{-2.f, 1.f, 0.f}},
                                &scene.default_cone_mat);
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

