#include "Window.h"
#include "modeling/MD_Renderer.h"
#include "modeling/MD_Shader.h"
#include "modeling/MD_Scene.h"
#include "controller.h"
#include "interpreter/scene_serializer.h"

void Window::sdlSetAttributes() {
    // SDL_Init is called once here; don't call it again in the constructor.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(SDL_GetError());
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
}

Window::Window(const std::string& name, int x, int y)
{
    sdlSetAttributes();

    // Use the current display mode to get the native resolution.
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
        throw std::runtime_error(SDL_GetError());

    width  = dm.w;
    height = dm.h;

    win = SDL_CreateWindow(name.c_str(), x, y, width, height,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!win)
        throw std::runtime_error(SDL_GetError());

    // On HiDPI/Retina displays the drawable pixel count differs from the
    // logical window size — read it back so every GL calculation is correct.
    SDL_GL_GetDrawableSize(win, &width, &height);

    // Derive panel dimensions from the actual pixel resolution.
    panel_top = static_cast<int>(height * kTopPanelFrac);
    panel_bottom = static_cast<int>(height * kBottomPanelFrac);
    panel_left = static_cast<int>(width  * kLeftPanelFrac);
    panel_right = static_cast<int>(width  * kRightPanelFrac);
}

Window::~Window() {
    if (gl_context) SDL_GL_DeleteContext(gl_context);
    if (win) SDL_DestroyWindow(win);
    SDL_Quit();
}

void Window::winInitGl() {
    gl_context = SDL_GL_CreateContext(win);
    if (!gl_context) {
        std::cerr << "error: could not create GL context\n";
        std::exit(EXIT_FAILURE);
    }
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "error: glad failed to load\n";
        std::exit(EXIT_FAILURE);
    }
    if (SDL_GL_SetSwapInterval(1) < 0)
        std::cerr << "warning: unable to set VSync\n";
}

// main loop 

void Window::updateViewport() {
    // The GL scene lives in the rectangle that remains after all four panels
    // are carved out.  Origin is bottom-left in OpenGL, so the viewport's
    // y-offset equals panel_bottom.
    viewport_w = width  - panel_left - panel_right;
    viewport_h = height - panel_top  - panel_bottom;
    glViewport(panel_left, panel_bottom, viewport_w, viewport_h);
}


void Window::winRun(GUI& gui, Controller& controller,
                    MD_Scene& scene, MD_Renderer& renderer,
                    SharedResources& shared, ModelingResources& modeling) {
    glEnable(GL_DEPTH_TEST);
    updateViewport();

    MD_Camera& camera = renderer.getCameraMain();
    camera = MD_Camera(Vec3f(0.f, 2.f, 6.f), Vec3f(0.f, 0.f, -1.f));
    camera.setAspect(static_cast<float>(viewport_w) / viewport_h);

    MD_Shader shader(SHADER_DIR "trs_shader.vs", SHADER_DIR "trs_shader.fs");
    scene.loadDefaultScene(modeling);

    Input input;
    Uint64 last = SDL_GetPerformanceCounter();
    float dt = 0.f;
    bool running = true;

    while (running) {
        input.beginFrame();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            gui.processEvent(e);
            if (!ImGui::GetIO().WantCaptureKeyboard && !ImGui::GetIO().WantCaptureMouse)
                input.handleEvent(e);
            if (e.type == SDL_QUIT)
                running = false;
            if (e.type == SDL_WINDOWEVENT &&
                e.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_GL_GetDrawableSize(win, &width, &height);
                int lw, lh;
                SDL_GetWindowSize(win, &lw, &lh);
                panel_top = static_cast<int>(height * kTopPanelFrac);
                panel_bottom = static_cast<int>(height * kBottomPanelFrac);
                panel_left = static_cast<int>(width  * kLeftPanelFrac);
                panel_right = static_cast<int>(width  * kRightPanelFrac);
                updateViewport();
                gui.resize(lw, lh, panel_bottom, panel_top, panel_left, panel_right);
                camera.setAspect(static_cast<float>(viewport_w) / viewport_h);
            }
        }

        if (input.quitPressed() || input.isKeyPressed(SDL_SCANCODE_ESCAPE))
            running = false;

        // timing
        Uint64 now = SDL_GetPerformanceCounter();
        dt = static_cast<float>(
            static_cast<double>(now - last) / SDL_GetPerformanceFrequency());
        last = now;

        // pick
        int picked = -1;
        if (input.isMousePressed(SDL_BUTTON_RIGHT)) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            bool in_viewport = mx > panel_left
                            && mx < (panel_left + viewport_w)
                            && my > panel_top
                            && my < (panel_top  + viewport_h);
            if (in_viewport)
                picked = renderer.pickAt(scene, mx, my,
                                         panel_left, panel_bottom,
                                         viewport_w, viewport_h);
        }

        // mode sync
        if (input.isKeyPressed(SDL_SCANCODE_T)) gui.setSelectedTool(CtrlMode::TRANSLATE);
        if (input.isKeyPressed(SDL_SCANCODE_R)) gui.setSelectedTool(CtrlMode::ROTATE);
        if (input.isKeyPressed(SDL_SCANCODE_S)) gui.setSelectedTool(CtrlMode::SCALE);
        if (input.isKeyPressed(SDL_SCANCODE_P)) gui.setSelectedTool(CtrlMode::CAMERA);
        controller.setMode(gui.getSelectedTool());

        // selected object
        MD_Object* selected = nullptr;
        if (!scene.selected_is_light && scene.selected_obj_index >= 0)
            selected = scene.getObject(scene.selected_obj_index);

        controller.ctrlUpdate(input, camera, selected, scene, picked, dt);

        // render
        glDisable(GL_SCISSOR_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_SCISSOR_TEST);
        glScissor(panel_left, panel_bottom, viewport_w, viewport_h);
        glClearColor(0.35f, 0.35f, 0.38f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        renderer.render(scene, shader);

        gui.begin();
        gui.drawPanelTop(scene, renderer.getCameraMain(), shared, modeling);
        gui.drawPanelBottom(scene, camera);
        gui.drawPanelRight(scene, modeling, shared);
        gui.drawPanelLeft(scene, modeling, shared);
        gui.render();

        SDL_GL_SwapWindow(win);
    }
}