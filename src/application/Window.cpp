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


void Window::sdlSetAttributes() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
}

Window::Window(const std::string& name, int _width, int _height, int _x, int _y)
    : width(_width), height(_height)
{
    sdlSetAttributes();
    win = SDL_CreateWindow(name.c_str(), _x, _y, _width, _height,
                           SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
}

Window::~Window() {
    if (gl_context) SDL_GL_DeleteContext(gl_context);
    if (win)        SDL_DestroyWindow(win);
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
    viewport_w = width  - panel_width;
    viewport_h = height - panel_height;
    glViewport(0, 0, viewport_w, viewport_h);
}

void Window::winRun() {
    winInitGl();  // make sure GL context is live before any GL call

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // ── renderer ──────────────────────────────────────────────────────────
    MD_Renderer renderer;

    // ── camera ────────────────────────────────────────────────────────────
    MD_Camera& camera = renderer.getCameraMain();
    camera = MD_Camera(Vec3f(0.f, 2.f, 6.f), Vec3f(0.f, 0.f, -1.f));
    camera.setAspect((float)width / height);

    // ── shader ────────────────────────────────────────────────────────────
    MD_Shader shader(SHADER_DIR "trs_shader.vs",
                     SHADER_DIR "trs_shader.fs");


    // ── textures ──────────────────────────────────────────────────────────
    MD_Texture texture(IMG_DIR "texture-background.jpg");
    texture.init();
    texture.load_Image();

    MD_Texture texture2(IMG_DIR "cracked_tx.jpg");
    texture2.init();
    texture2.load_Image();

    // ── scene ─────────────────────────────────────────────────────────────
    MD_Scene scene;

    MD_Sphere sp1(5, 5);
    scene.createObject(&sp1, { {30.f, 0.f, 30.f} }, &texture);

    MD_Sphere sp(25, 25);
    scene.createObject(&sp, { {0.f, 1.f, 0.f} }, &texture);

    MD_Quad quad0(10, 10);
    scene.createObject(&quad0, { {0.f, 0.f, 0.f} }, &texture);   // TODO: bind texture

    MD_Quad quad1(10, 10);
    scene.createObject(&quad1, { {0.f, 6.f, 0.f} }, &texture);   // TODO: bind texture2

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

    // ── mouse state ───────────────────────────────────────────────────────
    int ax = 0, ay = 0;
    SDL_GetMouseState(&ax, &ay);

    SDL_Event e;

    // ── main loop ─────────────────────────────────────────────────────────
    while (!stop) {

        // -- events --------------------------------------------------------
        while (SDL_PollEvent(&e)) {
            switch (e.type) {

                case SDL_QUIT:
                    stop = true;
                    break;

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                        stop = true;

                    if (e.key.keysym.sym == SDLK_TAB) {
                        scene.nextObject();
                        std::cout << "Selected object: "
                                  << scene.selected_index << '\n';
                    }
                    break;

                case SDL_MOUSEMOTION: {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    if (e.motion.state & SDL_BUTTON_LMASK) {
                        MD_Camera::CameraInput mi{};
                        mi.xoffset = static_cast<float>(ax - x);
                        mi.yoffset = static_cast<float>(ay - y);
                        mi.speed   = 5.f;
                        camera.update(mi);
                    }
                    ax = x;
                    ay = y;
                    break;
                }

                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                        width  = e.window.data1;
                        height = e.window.data2;
                        glViewport(0, 0, width, height);
                        camera.setAspect((float)width / height);
                    }
                    break;
            }
        }

        // -- timing --------------------------------------------------------
        Uint64 now = SDL_GetPerformanceCounter();
        deltaTime  = static_cast<double>(now - last)
                   / SDL_GetPerformanceFrequency();
        last = now;

        // -- keyboard ------------------------------------------------------
        const Uint8* ks = SDL_GetKeyboardState(nullptr);

        // camera movement
        MD_Camera::CameraInput ci{};
        ci.w     = ks[SDL_SCANCODE_W];
        ci.s     = ks[SDL_SCANCODE_S];
        ci.a     = ks[SDL_SCANCODE_A];
        ci.d     = ks[SDL_SCANCODE_D];
        ci.up    = ks[SDL_SCANCODE_SPACE];
        ci.down  = ks[SDL_SCANCODE_N];
        ci.speed = static_cast<float>(4.0 * deltaTime);
        camera.update(ci);

        // object transform
        auto& objects = scene.getObjects();
        if (!objects.empty()) {
            MD_Object* sel = scene.getObject(scene.selected_index);
            if (sel) {
                const float mv = 5.0f  * static_cast<float>(deltaTime);
                const float rv = 60.0f * static_cast<float>(deltaTime);
                const float sv = 1.0f  * static_cast<float>(deltaTime);

                if (ks[SDL_SCANCODE_UP])       sel->trs.translation += Vec3f( 0,  mv,  0);
                if (ks[SDL_SCANCODE_DOWN])      sel->trs.translation += Vec3f( 0, -mv,  0);
                if (ks[SDL_SCANCODE_LEFT])      sel->trs.translation += Vec3f(-mv,  0,  0);
                if (ks[SDL_SCANCODE_RIGHT])     sel->trs.translation += Vec3f( mv,  0,  0);
                if (ks[SDL_SCANCODE_PAGEUP])    sel->trs.translation += Vec3f( 0,   0,  mv);
                if (ks[SDL_SCANCODE_PAGEDOWN])  sel->trs.translation += Vec3f( 0,   0, -mv);

                if (ks[SDL_SCANCODE_Q])  sel->trs.rotation += Vec3f(0,  rv, 0);
                if (ks[SDL_SCANCODE_E])  sel->trs.rotation += Vec3f(0, -rv, 0);

                if (ks[SDL_SCANCODE_Z])  sel->trs.scale *= (1.0f + sv);
                if (ks[SDL_SCANCODE_X])  sel->trs.scale *= (1.0f - sv);
            }
        }

        // -- render --------------------------------------------------------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.render(scene, shader);
        SDL_GL_SwapWindow(win);
    }
}

