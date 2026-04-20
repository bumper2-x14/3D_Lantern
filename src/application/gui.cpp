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

void GUI::drawPanels(int width, int height, int panel_top, int panel_bottom,
                     int panel_left, int panel_right) {

    int viewport_h = height - panel_top - panel_bottom;
    int viewport_w = width  - panel_left - panel_right;

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    // ── TOP ─────────────────────────
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, panel_top));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("TopBar", nullptr, flags);
    ImGui::Text("Toolbar");
    ImGui::End();
    ImGui::PopStyleVar();

    // ── BOTTOM ──────────────────────
    ImGui::SetNextWindowPos(ImVec2(0, height - panel_bottom));
    ImGui::SetNextWindowSize(ImVec2(width, panel_bottom));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("Status", nullptr, flags);
    ImGui::Text("Status bar");
    ImGui::End();
    ImGui::PopStyleVar();

    // ── LEFT ────────────────────────
    ImGui::SetNextWindowPos(ImVec2(0, panel_top));
    ImGui::SetNextWindowSize(ImVec2(panel_left, viewport_h));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("Outliner", nullptr, flags);
    ImGui::Text("Scene objects");
    ImGui::End();
    ImGui::PopStyleVar();

    // ── RIGHT ───────────────────────
    ImGui::SetNextWindowPos(ImVec2(width - panel_right, panel_top));
    ImGui::SetNextWindowSize(ImVec2(panel_right, viewport_h));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("Inspector", nullptr, flags);
    ImGui::Text("Properties");
    ImGui::End();
    ImGui::PopStyleVar();
}