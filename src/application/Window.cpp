#include "Window.h"
#include "modeling/MD_Renderer.h"
#include "modeling/MD_Shader.h"
#include "modeling/MD_Scene.h"
#include "modeling/MD_Sphere.h"
#include "modeling/MD_Quad.h"
#include "modeling/MD_Cylinder.h"
#include "modeling/MD_Circle.h"
#include "modeling/MD_Cone.h"
#include "modeling/MD_Torus.h"
#include "controller.h"


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

// ── main loop ─────────────────────────────────────────────────────────────────

void Window::updateViewport() {
    // The GL scene lives in the rectangle that remains after all four panels
    // are carved out.  Origin is bottom-left in OpenGL, so the viewport's
    // y-offset equals panel_bottom.
    viewport_w = width  - panel_left - panel_right;
    viewport_h = height - panel_top  - panel_bottom;
    glViewport(panel_left, panel_bottom, viewport_w, viewport_h);
}


void Window::winRun() {
    winInitGl();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    updateViewport();   // single source of truth for viewport + camera aspect

    // ── renderer ──────────────────────────────────────────────────────────
    MD_Renderer renderer;

    // ── camera ────────────────────────────────────────────────────────────
    MD_Camera& camera = renderer.getCameraMain();
    camera = MD_Camera(Vec3f(0.f, 2.f, 6.f), Vec3f(0.f, 0.f, -1.f));
    camera.setAspect(static_cast<float>(viewport_w) / viewport_h);
    // ── shader ────────────────────────────────────────────────────────────
    MD_Shader shader(SHADER_DIR "light_Shaders.vs",
                     SHADER_DIR "light_Shaders.fs");

    // ── textures ──────────────────────────────────────────────────────────
    MD_Texture texture(IMG_DIR "bluePng.png");
    texture.init();
    texture.load_Image();

    MD_Texture texture2(IMG_DIR "coolJoshHomme.jfif");
    texture2.init();
    texture2.load_Image();

    // ── scene ─────────────────────────────────────────────────────────────
    MD_Scene scene;

    MD_Sphere sp1(5, 5);
    scene.createObject(&sp1, { {30.f, 0.f, 30.f} }, &texture);

    MD_Sphere sp(25, 25);
    scene.createObject(&sp, { {0.f, 1.f, 0.f} }, &texture);

    MD_Quad quad0(10, 10);
    scene.createObject(&quad0, { {0.f, 0.f, 0.f} }, &texture);

    MD_Quad quad1(10, 10);
    scene.createObject(&quad1, { {0.f, 6.f, 0.f} }, &texture);

    MD_Cylinder cylinder(25);
    scene.createObject(&cylinder, { {2.f, 2.f, 0.f}, {0.5f, 2.f, 0.5f} }, &texture);

    MD_Disk circle(40);
    scene.createObject(&circle, { {0.f, 4.f, 0.f} }, &texture);

    MD_Cone cone;
    scene.createObject(&cone, { {-2.f, 1.f, 0.f} }, &texture2);

    MD_Torus torus;
    scene.createObject(&torus, { {-7.f, 1.f, 0.f} }, &texture2);

    // ── timing ────────────────────────────────────────────────────────────
    Uint64 last = SDL_GetPerformanceCounter();
    double deltaTime = 0.0;

    Input input;
    Controller controller;

    while (!stop) {
        // ── input ─────────────────────────────────────────
        input.update();

        if (input.quitPressed() || input.isKeyPressed(SDL_SCANCODE_ESCAPE))
            stop = true;

        // ── timing ────────────────────────────────────────
        Uint64 now = SDL_GetPerformanceCounter();
        deltaTime  = static_cast<double>(now - last) / SDL_GetPerformanceFrequency();
        last = now;

        // ── controller ────────────────────────────────────
        MD_Object* selected = nullptr;
        if (!scene.getObjects().empty())
            selected = scene.getObject(scene.selected_index);

        controller.ctrlUpdate(input, camera, selected, scene,
                              static_cast<float>(deltaTime));

        // ── SDL events ────────────────────────────────────
        // No resize handling needed in fullscreen; keep the loop for any
        // other SDL events your input system doesn't consume.
        SDL_Event e;
        while (SDL_PollEvent(&e)) { /* forwarded to input if needed */ }
              // ── render ────────────────────────────────────────
        // Full-screen clear paints the panel background colour.
        glDisable(GL_SCISSOR_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Scissor to the viewport region and clear with the scene colour.
        glEnable(GL_SCISSOR_TEST);
        glScissor(panel_left, panel_bottom, viewport_w, viewport_h);
        glClearColor(0.35f, 0.35f, 0.38f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        renderer.render(scene, shader);
        SDL_GL_SwapWindow(win);
    }
}
