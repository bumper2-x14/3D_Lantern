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

GLuint loadTexture(const char* path) {
    int w, h, channels;
    unsigned char* data = stbi_load(path, &w, &h, &channels, 4);

    if (!data) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return 0;
    }

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}

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

    icon_sphere   = loadTexture("../../../img/Sphere.png");
    icon_cylinder = loadTexture("../../../img/Cylinder.png");
    icon_cone     = loadTexture("../../../img/Cone.png");
    icon_torus    = loadTexture("../../../img/Torus.png");
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

    ImGui::Begin("Outliner", nullptr, flags);

    ImGui::Text("Scene objects");

    ImGui::Separator();
    ImGui::Text("Add shape:");

    enum ShapeChoice {
        NONE = -1,
        SPHERE,
        CYLINDER,
        CONE,
        TORUS
    };

    static int selectedShape = NONE;
    static float pos[3] = {0.f, 1.f, 0.f};
    static float color[3] = {0.5f, 0.5f, 0.5f};

    // Sphere icon
    if (ImGui::ImageButton("sphere_icon",
                           (ImTextureID)(intptr_t)icon_sphere,
                           ImVec2(45, 45))) {
        selectedShape = SPHERE;
        pos[0] = 0.f; pos[1] = 1.f; pos[2] = 0.f;
        ImGui::OpenPopup("Create Shape");
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Sphere");

    ImGui::SameLine();

    // Cone icon
    if (ImGui::ImageButton("cone_icon",
                           (ImTextureID)(intptr_t)icon_cone,
                           ImVec2(45, 45))) {
        selectedShape = CONE;
        pos[0] = 0.f; pos[1] = 1.f; pos[2] = 0.f;
        ImGui::OpenPopup("Create Shape");
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Cone");

    ImGui::SameLine();

    // Torus icon
    if (ImGui::ImageButton("torus_icon",
                           (ImTextureID)(intptr_t)icon_torus,
                           ImVec2(45, 45))) {
        selectedShape = TORUS;
        pos[0] = 0.f; pos[1] = 1.f; pos[2] = 0.f;
        ImGui::OpenPopup("Create Shape");
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Torus");

    ImGui::SameLine();

    // Cylinder icon
    if (ImGui::ImageButton("cylinder_icon",
                           (ImTextureID)(intptr_t)icon_cylinder,
                           ImVec2(45, 45))) {
        selectedShape = CYLINDER;
        pos[0] = 0.f; pos[1] = 1.f; pos[2] = 0.f;
        ImGui::OpenPopup("Create Shape");
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Cylinder");

    // Popup
    if (ImGui::BeginPopupModal("Create Shape", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        const char* shapeName = "Shape";

        if (selectedShape == SPHERE) shapeName = "Sphere";
        else if (selectedShape == CYLINDER) shapeName = "Cylinder";
        else if (selectedShape == CONE) shapeName = "Cone";
        else if (selectedShape == TORUS) shapeName = "Torus";

        ImGui::Text("Create %s", shapeName);
        ImGui::Separator();

        ImGui::DragFloat3("Position", pos, 0.1f);
        ImGui::ColorEdit3("Color", color);

        if (ImGui::Button("Create")) {
            MD_Material* mat = new MD_Material(
                Vec3f(color[0], color[1], color[2]),
                MD_Material::MatType::DIFFUSE
            );

            MD_Object* created = nullptr;

            if (selectedShape == SPHERE) {
                created = scene.createObject(
                    "Sphere",
                    &scene.default_sphere,
                    TRSDataf{{pos[0], pos[1], pos[2]}},
                    mat
                );
            }

            else if (selectedShape == CYLINDER) {
                created = scene.createObject(
                    "Cylinder",
                    &scene.default_cylinder,
                    TRSDataf{{pos[0], pos[1], pos[2]}},
                    mat
                );
            }

            else if (selectedShape == CONE) {
                created = scene.createObject(
                    "Cone",
                    &scene.default_cone,
                    TRSDataf{{pos[0], pos[1], pos[2]}},
                    mat
                );
            }

            else if (selectedShape == TORUS) {
                created = scene.createObject(
                    "Torus",
                    &scene.default_torus,
                    TRSDataf{{pos[0], pos[1], pos[2]}},
                    mat
                );
            }

            if (created) {
                scene.selected_obj_index = (int)scene.objects.size() - 1;
                scene.selected_light_index = -1;
                scene.selected_is_light = false;
                scene.show_selected = true;
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
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

