#include "gui.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "modeling/MD_Sphere.h"
#include "modeling/MD_Cylinder.h"
#include "modeling/MD_Cone.h"
#include "modeling/MD_Torus.h"
#include "modeling/MD_ModelShape.h"
#include "assets/color_texture.h"
#include "assets/checker_texture.h"
#include "assets/image_texture.h"
#include "assets/perlin_texture.h"

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

void GUI::drawPanelTop(MD_Scene& scene, MD_Camera& camera,
                       SharedResources& shared, ModelingResources& modeling) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, panel_top));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("TopBar", nullptr, flags);

    ImGui::Text("3D Lantern");
    ImGui::SameLine();

    float rightAlign = ImGui::GetWindowWidth() - 600;
    ImGui::SetCursorPosX(rightAlign);

    int selected = 0;
    if (selected_tool == CtrlMode::CAMERA)    selected = 0;
    else if (selected_tool == CtrlMode::TRANSLATE) selected = 1;
    else if (selected_tool == CtrlMode::ROTATE)    selected = 2;
    else if (selected_tool == CtrlMode::SCALE)     selected = 3;

    ImGui::RadioButton("Camera",    &selected, 0); ImGui::SameLine();
    ImGui::RadioButton("Translate", &selected, 1); ImGui::SameLine();
    ImGui::RadioButton("Rotate",    &selected, 2); ImGui::SameLine();
    ImGui::RadioButton("Scale",     &selected, 3); ImGui::SameLine();

    if (selected == 0) selected_tool = CtrlMode::CAMERA;
    else if (selected == 1) selected_tool = CtrlMode::TRANSLATE;
    else if (selected == 2) selected_tool = CtrlMode::ROTATE;
    else if (selected == 3) selected_tool = CtrlMode::SCALE;

    // buttons
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 240);

    if (ImGui::Button("Save Scene", ImVec2(110, panel_top - 10))) {
        SerializerSettings ss = render_settings;
        ss.aspectRatio = (double)render_settings.width / render_settings.height;
        std::string lnt = SceneSerializer::serialize(scene, camera, shared, modeling, ss);
        std::ofstream f(scene_output);
        if (f) { f << lnt; std::cout << "Scene saved to " << scene_output << '\n'; }
        else std::cerr << "Failed to save scene to " << scene_output << '\n';
    }
    ImGui::SameLine();
    if (ImGui::Button(">> Render", ImVec2(110, panel_top - 10)))
        ImGui::OpenPopup("Render Settings");

    // popup
    ImGui::SetNextWindowSize(ImVec2(350, 0), ImGuiCond_Always);
    if (ImGui::BeginPopupModal("Render Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Render Settings");
        ImGui::Separator();

        ImGui::DragInt("Width",   &render_settings.width,   1.f, 100, 4096);
        ImGui::DragInt("Height",  &render_settings.height,  1.f, 100, 4096);
        ImGui::DragInt("Samples", &render_settings.samples, 1.f, 1,   512);
        ImGui::DragInt("Depth",   &render_settings.depth,   1.f, 1,   64);

        float bg[3] = { render_settings.background.x,
                        render_settings.background.y,
                        render_settings.background.z };
        if (ImGui::ColorEdit3("Background", bg))
            render_settings.background = {bg[0], bg[1], bg[2]};

        ImGui::Separator();
        ImGui::InputText("Scene path",  scene_output,  sizeof(scene_output));
        ImGui::InputText("Output path", render_output, sizeof(render_output));
        ImGui::Checkbox("Multithreaded", &render_threaded);
        ImGui::Separator();

        if (ImGui::Button("Render", ImVec2(-1, 35))) {
            render_settings.aspectRatio = (double)render_settings.width / render_settings.height;
            std::string lnt = SceneSerializer::serialize(scene, camera, shared, modeling, render_settings);
            Interpreter interp(lnt, true);
            RT_Renderer& rt = interp.makeRayTracer();
            rt.render(render_threaded);
            rt.writePPM(render_output);
            std::cout << "Render saved to " << render_output << '\n';
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(-1, 35)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

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

void GUI::drawPanelLeft(MD_Scene& scene, ModelingResources& modeling, SharedResources& shared){
    ImGui::SetNextWindowPos(ImVec2(0, panel_top));
    ImGui::SetNextWindowSize(ImVec2(panel_left, height - panel_top - panel_bottom));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::Begin("Scene", nullptr, flags);

    ImGui::Text("Objects");
    ImGui::Separator();

    enum ShapeChoice { NONE = -1, SPHERE, CYLINDER, CONE, TORUS, QUAD, DISK, BOX };
    static int selectedShape = NONE;
    static float pos[3] = {0.f, 1.f, 0.f};
    static float rot[3] = {0.f, 0.f, 0.f};
    static float scale[3] = {1.f, 1.f, 1.f};
    static float color[3] = {0.5f, 0.5f, 0.5f};

    float btn_w = (ImGui::GetContentRegionAvail().x - 10) / 2.f;

    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.05f, 0.5f));
    if (ImGui::Button("+ Box", ImVec2(-1, 30))) { selectedShape = BOX; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Sphere", ImVec2(-1, 30))) { selectedShape = SPHERE; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Cone", ImVec2(-1, 30))) { selectedShape = CONE; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Torus", ImVec2(-1, 30))) { selectedShape = TORUS; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Cylinder", ImVec2(-1, 30))) { selectedShape = CYLINDER; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Plane", ImVec2(-1, 30))) { selectedShape = QUAD; ImGui::OpenPopup("Create Shape"); }
    if (ImGui::Button("+ Disk", ImVec2(-1, 30))) { selectedShape = DISK; ImGui::OpenPopup("Create Shape"); }
    

    ImGui::Separator();
    ImGui::Text("Meshes");
    ImGui::Separator();
    if (ImGui::Button("+ Mesh",  ImVec2(-1, 30))) ImGui::OpenPopup("Load Mesh");
    ImGui::Separator();
    ImGui::Text("Meshes");
    ImGui::Separator();
    if (ImGui::Button("+ PointLight", ImVec2(-1, 30))) ImGui::OpenPopup("Create Light");
    ImGui::PopStyleVar();

    // Popup unchanged
    if (ImGui::BeginPopupModal("Create Shape", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        const char* shapeName = selectedShape == BOX ? "Box"
                            : selectedShape == SPHERE ? "Sphere"
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
        ImGui::DragFloat3("Rotation", rot,   0.5f);
        ImGui::DragFloat3("Scale", scale, 0.05f, 0.01f, 100.f);
        ImGui::ColorEdit3("Color", color);

        if (ImGui::Button("Create")) {
            const std::string name = (obj_name[0] != '\0') ? obj_name : shapeName;

            /*
            MD_Material* mat = new MD_Material(
                Vec3f(color[0], color[1], color[2]),
                MD_Material::MatType::DIFFUSE);
            */

            TRSDataf trs{
                {pos[0],   pos[1],   pos[2]},   // translation
                {scale[0], scale[1], scale[2]}, // scale
                {rot[0],   rot[1],   rot[2]}    // rotation
            };

            MD_Object* created = nullptr;
            if (selectedShape == SPHERE)   created = scene.createObject(name, &scene.default_sphere, trs, scene.default_material);
            else if (selectedShape == BOX) created = scene.createObject(name, &scene.default_box, trs, scene.default_material);
            else if (selectedShape == CYLINDER) created = scene.createObject(name, &scene.default_cylinder, trs, scene.default_material);
            else if (selectedShape == CONE) created = scene.createObject(name, &scene.default_cone, trs, scene.default_material);
            else if (selectedShape == TORUS) created = scene.createObject(name, &scene.default_torus, trs, scene.default_material);
            else if (selectedShape == QUAD) created = scene.createObject(name, &scene.default_quad, trs, scene.default_material);
            else if (selectedShape == DISK) created = scene.createObject(name, &scene.default_disk, trs, scene.default_material);

            if (created) {
                scene.selected_obj_index = (int)scene.objects.size() - 1;
                scene.selected_light_index = -1;
                scene.selected_is_light = false;
                scene.show_selected = true;
            }
            obj_name[0] = '\0';  // clear for next time
            // reset transforms
            pos[0] = 0.f; pos[1] = 1.f; pos[2] = 0.f;
            rot[0] = 0.f; rot[1] = 0.f; rot[2] = 0.f;
            scale[0] = 1.f; scale[1] = 1.f; scale[2] = 1.f;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            obj_name[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // Create Light popup
    if (ImGui::BeginPopupModal("Create Light", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static float lpos[3]   = {0.f, 3.f, 0.f};
        static float lcolor[3] = {1.f, 1.f, 1.f};
        static float intensity = 1.f;

        ImGui::Text("Create Point Light");
        ImGui::Separator();
        ImGui::DragFloat3("Position",  lpos,   0.1f);
        ImGui::ColorEdit3("Color",     lcolor);
        ImGui::DragFloat ("Intensity", &intensity, 0.05f, 0.f, 20.f);

        if (ImGui::Button("Create")) {
            scene.createPointLight(
                Vec3f(lpos[0], lpos[1], lpos[2]),
                Color(lcolor[0], lcolor[1], lcolor[2]),
                intensity);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    // Load Mesh popup
    if (ImGui::BeginPopupModal("Load Mesh", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char mesh_name[64] = "";
        static char mesh_path[256] = "";
        static float mpos[3]  = {0.f, 0.f, 0.f};
        static float mscale[3] = {1.0f, 1.0f, 1.0f};
        static float mrotate[3] = {0.0f, 0.0f, 0.0f};
        static float mcolor[3] = {0.5f, 0.5f, 0.5f};

        ImGui::Text("Load OBJ Mesh");
        ImGui::Separator();
        ImGui::InputText("Name", mesh_name, sizeof(mesh_name));
        ImGui::InputText("Path", mesh_path, sizeof(mesh_path));
        ImGui::DragFloat3("Position", mpos, 0.1f);
        ImGui::DragFloat3("Scale", mscale, 0.1f);
        ImGui::DragFloat3("Rotation", mrotate, 0.1f);
        ImGui::ColorEdit3("Color", mcolor);

        TRSDataf mtrs{
                {pos[0],   pos[1],   pos[2]},   // translation
                {scale[0], scale[1], scale[2]}, // scale
                {rot[0],   rot[1],   rot[2]}    // rotation
        };

        if (ImGui::Button("Load")) {
            if (mesh_path[0] != '\0') {
                const std::string name = (mesh_name[0] != '\0') ? mesh_name : "Mesh";

                // 1. load into shared resources
                Model* model = shared.loadModel(name, mesh_path);
                if (model) {
                    MD_ModelShape* shape = modeling.addModelShape(name, model);
                    /*
                    MD_Material* mat = modeling.addMaterial(name + "_mat",
                        new MD_Material(Vec3f(mcolor[0], mcolor[1], mcolor[2]),
                                        MD_Material::MatType::DIFFUSE));
                    */
                    scene.createObject(name, shape, mtrs, scene.default_material);
                    scene.selected_obj_index = (int)scene.objects.size() - 1;
                    scene.selected_is_light = false;
                    scene.show_selected = true;
                } else {
                    std::cerr << "Failed to load OBJ: " << mesh_path << '\n';
                }
            }
            mesh_name[0] = '\0';
            mesh_path[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            mesh_name[0] = '\0';
            mesh_path[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }


    ImGui::Separator();
    ImGui::End();
    ImGui::PopStyleVar();
}

void GUI::drawPanelRight(MD_Scene& scene, ModelingResources& modeling, SharedResources& shared){
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

    if (ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("sel_obj=%d is_light=%d", scene.selected_obj_index, (int)scene.selected_is_light);

        if (!scene.selected_is_light && scene.selected_obj_index >= 0) {
            MD_Object* obj = scene.getSelectedObject();
            if (obj) {
                ImGui::Text("Object: %s", obj->name.c_str());
                ImGui::Separator();
                ImGui::Text("Material");

                const std::string cur_mat = modeling.findMaterialName(obj->getMaterial());
                ImGui::Text("Current: %s", cur_mat.empty() ? "none" : cur_mat.c_str());

                static int mat_idx = 0;
                const auto& mats = modeling.getMaterials();
                std::vector<const char*> mat_names;
                std::vector<MD_Material*> mat_ptrs;
                for (auto& [name, mat] : mats) {
                    mat_names.push_back(name.c_str());
                    mat_ptrs.push_back(mat);
                }

                if (!mat_names.empty()) {
                    ImGui::Combo("##mat_select", &mat_idx, mat_names.data(), (int)mat_names.size());
                    if (ImGui::Button("Apply Material", ImVec2(-1, 30)))
                        if (mat_idx < (int)mat_ptrs.size())
                            obj->setMaterial(mat_ptrs[mat_idx]);
                } else {
                    ImGui::TextDisabled("No materials in registry");
                }
            }
        } else if (scene.selected_is_light && scene.selected_light_index >= 0) {
            MD_PointLight* light = scene.getSelectedLight();
            if (light) {
                ImGui::Text("Light %d", scene.selected_light_index);
                ImGui::Separator();
                Vec3f pos = light->getPosition();
                float p[3] = {pos.x, pos.y, pos.z};
                if (ImGui::DragFloat3("Position", p, 0.1f))
                    light->setPosition(Vec3f(p[0], p[1], p[2]));
                float intensity = light->getIntensity();
                if (ImGui::DragFloat("Intensity", &intensity, 0.05f, 0.f, 20.f))
                    light->setIntensity(intensity);
            }
        } else {
            ImGui::TextDisabled("Nothing selected");
        }
    }

    ImGui::Separator();
    ImGui::Separator();
    if (ImGui::BeginTabBar("Factory")) {

        // ── TEXTURE TAB ──────────────────────────────────────────────────
        if (ImGui::BeginTabItem("Texture")) {
            static char tex_name[64] = "";
            static int tex_type = 0; // 0=Color, 1=Checker, 2=Image, 3=Noise, 4=Turbulence, 5=Marble, 6=Wood, 7=Warped
            static float tex_color[3]  = {1.f, 1.f, 1.f};
            static float tex_color2[3] = {0.f, 0.f, 0.f};
            static float tex_scale     = 1.f;
            static char  tex_path[256] = "";

            const char* tex_types[] = { "Color", "Checker", "Image", "Noise", "Turbulence", "Marble", "Wood", "Warped" };
            ImGui::InputText("Name##tex", tex_name, sizeof(tex_name));
            ImGui::Combo("Type", &tex_type, tex_types, IM_ARRAYSIZE(tex_types));

            switch (tex_type) {
                case 0: // Color
                    ImGui::ColorEdit3("Color##tc", tex_color);
                    break;
                case 1: // Checker
                    ImGui::ColorEdit3("Color A", tex_color);
                    ImGui::ColorEdit3("Color B", tex_color2);
                    ImGui::DragFloat("Scale##tc", &tex_scale, 0.05f, 0.01f, 100.f);
                    break;
                case 2: // Image
                    ImGui::InputText("Path##tex", tex_path, sizeof(tex_path));
                    break;
                default: // Perlin variants
                    ImGui::DragFloat("Scale##tc", &tex_scale, 0.05f, 0.01f, 100.f);
                    break;
            }

            if (ImGui::Button("Create Texture", ImVec2(-1, 30))) {
                const std::string name = (tex_name[0] != '\0') ? tex_name : tex_types[tex_type];
                if (!shared.hasTexture(name)) {
                    switch (tex_type) {
                        case 0: shared.addTexture(name, new ColorTexture(Vec3f(tex_color[0], tex_color[1], tex_color[2]))); break;
                        case 1: shared.addTexture(name, new CheckerTexture(Vec3f(tex_color[0], tex_color[1], tex_color[2]),
                                                                            Vec3f(tex_color2[0], tex_color2[1], tex_color2[2]), tex_scale)); break;
                        case 2: if (tex_path[0]) shared.addTexture(name, new ImageTexture(tex_path)); break;
                        case 3: shared.addTexture(name, new PerlinTexture(PerlinClassType::NOISE, tex_scale)); break;
                        case 4: shared.addTexture(name, new PerlinTexture(PerlinClassType::TURBULENCE, tex_scale)); break;
                        case 5: shared.addTexture(name, new PerlinTexture(PerlinClassType::MARBLE, tex_scale)); break;
                        case 6: shared.addTexture(name, new PerlinTexture(PerlinClassType::WOOD, tex_scale)); break;
                        case 7: shared.addTexture(name, new PerlinTexture(PerlinClassType::WARPED, tex_scale)); break;
                    }
                }
                tex_name[0] = '\0';
                tex_path[0] = '\0';
                tex_scale   = 1.f;
            }

            ImGui::Separator();
            ImGui::Text("Loaded textures:");
            for (auto& [name, tex] : shared.getTextures()) {
                ImGui::BulletText("%s", name.c_str());
            }

            ImGui::EndTabItem();
        }

        // ── MATERIAL TAB ─────────────────────────────────────────────────
        if (ImGui::BeginTabItem("Material")) {
            static char mat_name[64]    = "";
            static int  mat_type        = 0; // 0=Diffuse, 1=Specular, 2=Ambient, 3=Texture
            static float mat_color[3]   = {0.5f, 0.5f, 0.5f};
            static float mat_shininess  = 32.f;
            static char  mat_tex[64]    = ""; // texture name from shared

            const char* mat_types[] = { "Diffuse", "Specular", "Ambient", "Texture" };
            ImGui::InputText("Name##mat", mat_name, sizeof(mat_name));
            ImGui::Combo("Type##mat", &mat_type, mat_types, IM_ARRAYSIZE(mat_types));

            switch (mat_type) {
                case 0: // Diffuse
                case 2: // Ambient
                    ImGui::ColorEdit3("Color##mat", mat_color);
                    break;
                case 1: // Specular
                    ImGui::ColorEdit3("Color##mat", mat_color);
                    ImGui::DragFloat("Shininess", &mat_shininess, 0.5f, 1.f, 256.f);
                    break;
                case 3: // Texture
                    ImGui::InputText("Texture##mat", mat_tex, sizeof(mat_tex));
                    ImGui::Text("(must match a loaded texture name)");
                    break;
            }

            if (ImGui::Button("Create Material", ImVec2(-1, 30))) {
                const std::string name = (mat_name[0] != '\0') ? mat_name : "Material";
                if (!modeling.hasMaterial(name)) {
                    MD_Material* mat = nullptr;
                    switch (mat_type) {
                        case 0: mat = new MD_Material(name, Vec3f(mat_color[0], mat_color[1], mat_color[2]), MD_Material::MatType::DIFFUSE);  break;
                        case 1: mat = new MD_Material(name, Vec3f(mat_color[0], mat_color[1], mat_color[2]), MD_Material::MatType::SPECULAR, mat_shininess); break;
                        case 2: mat = new MD_Material(name, Vec3f(mat_color[0], mat_color[1], mat_color[2]), MD_Material::MatType::AMBIENT);  break;
                        case 3: {
                            Texture* tex = shared.getTexture(mat_tex);
                            if (tex) mat = new MD_Material(name, tex);
                            else ImGui::OpenPopup("tex_not_found");
                            break;
                        }
                    }
                    if (mat) modeling.addMaterial(name, mat);
                }
                mat_name[0] = '\0';
                mat_tex[0]  = '\0';
            }

            if (ImGui::BeginPopup("tex_not_found")) {
                ImGui::Text("Texture not found in shared resources.");
                if (ImGui::Button("OK")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            ImGui::Separator();
            ImGui::Text("Materials:");
            for (auto& [name, mat] : modeling.getMaterials()) {
                ImGui::BulletText("%s", name.c_str());
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
    ImGui::PopStyleVar();

}

