#include "gui.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "modeling/MD_Sphere.h"
#include "modeling/MD_Cylinder.h"
#include "modeling/MD_Cone.h"
#include "modeling/MD_Torus.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <glad/glad.h>

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

void GUI::resize(int w, int h, int pb, int pt, int pl, int pr) {
    width = w;
    height = h;
    panel_bottom = pb;
    panel_top = pt;
    panel_left = pl;
    panel_right = pr;
}

void GUI::drawPanelTop(MD_Scene& scene){
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, panel_top));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("TopBar", nullptr, flags);
    ImGui::Text("Toolbar");
    int selected = 0;

    //move cursor to the right side
    float rightAlign = ImGui::GetWindowWidth() - 600; 
    ImGui::SetCursorPosX(rightAlign);


    if (selected_tool == CtrlMode::CAMERA) selected = 0;
    else if (selected_tool == CtrlMode::TRANSLATE) selected = 1;
    else if (selected_tool == CtrlMode::ROTATE) selected = 2;
    else if (selected_tool == CtrlMode::SCALE) selected = 3;

    ImGui::RadioButton("move camera", &selected, 0); ImGui::SameLine();
    ImGui::RadioButton("move object", &selected, 1); ImGui::SameLine();
    ImGui::RadioButton("rotate object", &selected, 2); ImGui::SameLine();
    ImGui::RadioButton("scale object", &selected, 3);

    if (selected == 0) selected_tool = CtrlMode::CAMERA;
    else if (selected == 1) selected_tool = CtrlMode::TRANSLATE;
    else if (selected == 2) selected_tool = CtrlMode::ROTATE;
    else if (selected == 3) selected_tool = CtrlMode::SCALE;

    ImGui::End();
    ImGui::PopStyleVar();

   }

void GUI::drawPanelBottom(MD_Scene& scene, MD_Camera& camera){
    
    ImGui::SetNextWindowPos(ImVec2(0, height - panel_bottom));
    ImGui::SetNextWindowSize(ImVec2(width, panel_bottom));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

    ImGui::Begin("Status", nullptr, flags);

    MD_Object* selected = scene.getSelectedObject();

    if (selected) {
        Vec3f p = selected->trs.translation;

        ImGui::Text("Selected object: %s", selected->name.c_str());
        ImGui::SameLine();
        ImGui::Text(" | Position: %.2f %.2f %.2f", p.x, p.y, p.z);
    }
    else {
        ImGui::Text("Selected object: none");
    }

    Vec3f camPos = camera.getPosition();
    ImGui::SameLine();
    ImGui::Text(" | Camera: %.2f %.2f %.2f", camPos.x, camPos.y, camPos.z);

    ImGui::End();
    ImGui::PopStyleVar();
}

void GUI::drawPanelLeft(MD_Scene& scene){
    ImGui::SetNextWindowPos(ImVec2(0, panel_top));
    ImGui::SetNextWindowSize(ImVec2(panel_left, height - panel_top - panel_bottom));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::Begin("Scene", nullptr, flags);

    ImGui::Text("Objects");
    ImGui::Separator();

    enum ShapeChoice { NONE = -1, SPHERE, CYLINDER, CONE, TORUS, QUAD, DISK };
    static int selectedShape = NONE;
    static float pos[3] = {0.f, 1.f, 0.f};
    static float color[3] = {0.5f, 0.5f, 0.5f};

    float btn_w = (ImGui::GetContentRegionAvail().x - 10) / 2.f;

    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.05f, 0.5f));
    if (ImGui::Button("+ Sphere", ImVec2(-1, 30))) { selectedShape = SPHERE; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Cone", ImVec2(-1, 30))) { selectedShape = CONE; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Torus", ImVec2(-1, 30))) { selectedShape = TORUS; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Cylinder", ImVec2(-1, 30))) { selectedShape = CYLINDER; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Plane", ImVec2(-1, 30))) { selectedShape = QUAD; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Disk", ImVec2(-1, 30))) { selectedShape = DISK; ImGui::OpenPopup("Create Shape"); }
    ImGui::PopStyleVar();

    // Popup unchanged
    if (ImGui::BeginPopupModal("Create Shape", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        const char* shapeName = selectedShape == SPHERE ? "Sphere"
                            : selectedShape == CYLINDER ? "Cylinder"
                            : selectedShape == CONE ? "Cone"
                            : selectedShape == TORUS ? "Torus"
                            : selectedShape == QUAD ? "Plane"
                            : selectedShape == DISK ? "Disk" : "Shape";

        ImGui::Text("Create %s", shapeName);
        ImGui::Separator();

        static char obj_name[64] = "";
        ImGui::InputText("Name", obj_name, sizeof(obj_name));
        ImGui::DragFloat3("Position", pos, 0.1f);
        ImGui::ColorEdit3("Color", color);

        if (ImGui::Button("Create")) {
            const std::string name = (obj_name[0] != '\0') ? obj_name : shapeName;

            MD_Material* mat = new MD_Material(
                Vec3f(color[0], color[1], color[2]),
                MD_Material::MatType::DIFFUSE);

            MD_Object* created = nullptr;
            if (selectedShape == SPHERE)   created = scene.createObject(name, &scene.default_sphere,   TRSDataf{{pos[0],pos[1],pos[2]}}, mat);
            else if (selectedShape == CYLINDER) created = scene.createObject(name, &scene.default_cylinder, TRSDataf{{pos[0],pos[1],pos[2]}}, mat);
            else if (selectedShape == CONE) created = scene.createObject(name, &scene.default_cone,     TRSDataf{{pos[0],pos[1],pos[2]}}, mat);
            else if (selectedShape == TORUS) created = scene.createObject(name, &scene.default_torus,    TRSDataf{{pos[0],pos[1],pos[2]}}, mat);
            else if (selectedShape == QUAD) created = scene.createObject(name, &scene.default_quad, TRSDataf{{pos[0],pos[1],pos[2]}}, mat);
            else if (selectedShape == DISK) created = scene.createObject(name, &scene.default_disk,  TRSDataf{{pos[0],pos[1],pos[2]}}, mat);

            if (created) {
                scene.selected_obj_index = (int)scene.objects.size() - 1;
                scene.selected_light_index = -1;
                scene.selected_is_light = false;
                scene.show_selected = true;
            }
            obj_name[0] = '\0';  // clear for next time
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            obj_name[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::Separator();
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
                    snprintf(label, sizeof(label), "Object : %s##object_%d", obj->name.c_str(), i);

                    bool is_selected = (scene.selected_obj_index == i);
                    if (ImGui::Selectable(label, is_selected))
                        scene.setSelectedObject(obj->selectable ? i : -1);
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    i++;
                }
                ImGui::EndListBox();
                if (scene.selected_obj_index >= 0) {

                    if (ImGui::Button("🗑")) {
                        scene.removeObject(scene.selected_obj_index);
                        scene.selected_obj_index = -1;
                    }

                 }
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

