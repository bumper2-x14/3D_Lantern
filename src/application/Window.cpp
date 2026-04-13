/*
    DEV NOTE:
    To many unnecessary headers for this class
*/

#include "Window.h"
#include "modeling/MD_Camera.h"
#include "modeling/MD_Shader.h"
#include "modeling/MD_Sphere.h"
#include "modeling/MD_Quad.h"
#include "modeling/MD_Cylinder.h"
#include "modeling/MD_Circle.h"
#include "modeling/MD_Object.h"
#include "math/mat4.h"
#include "math/vec3.h"
#include "math/transform.h"
#include "modeling/MD_Renderer.h"
#include "modeling/MD_Material.h"
#include "modeling/MD_Texture.h"

void Window::sdlSetAttributes(){
    SDL_Init(SDL_INIT_VIDEO); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    #ifdef __APPLE__
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    #endif
}


/*
    DEV NOTE:
    window creation with uncheked and non portable parameters
    are dangerous for a default constructor 
*/
Window::Window(){
    sdlSetAttributes();
    win = SDL_CreateWindow( "window", 100, 100, 1920, 1080,
                SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE |
                                            SDL_WINDOW_OPENGL );
    width = 1920;
    height = 1080;
}

/*  
    DEV NOTE:
    Too many redundant constructors
*/

Window::Window(const std::string& name){
    sdlSetAttributes();
    win = SDL_CreateWindow(name.c_str(), 100, 100, 1920, 1080,
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE |
                                            SDL_WINDOW_OPENGL );
    width = 1920;
    height = 1080;
}


Window::Window(const std::string &name, int _width, int _height){
    sdlSetAttributes();
    win = SDL_CreateWindow(name.c_str(), 100, 100, _width, _height,
                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL );
    width = _width;
    height = _height;
}


Window::Window(const char* t, int _width, int _height){
    sdlSetAttributes();   
    win = SDL_CreateWindow(t, 100, 100, _width, _height,
                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL );
    width = _width;
    height = _height;
}


Window::Window(const std::string &name, int _width, int _height, int _x, int _y){
    sdlSetAttributes();  
    win = SDL_CreateWindow(name.c_str(), _x, _y, _width, _height,
                SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL );
    width = _width;
    height = _height;
}


void Window::winInitGl(){
    gl_context = SDL_GL_CreateContext(win);
    if( gl_context == NULL ){
        std::cout<<"error could not create gl context"<<std::endl;
        std::exit(EXIT_FAILURE);
    }
    //loading glad
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
        std::cout<<"loading glad failed"<<std::endl;
        std::exit(EXIT_FAILURE);    
    }
    if( SDL_GL_SetSwapInterval( 1 ) < 0 ){
        std::cout<<"Warning: Unable to set VSync!" << std::endl;
    }    
}


/*
    DEV NOTE:
    Abominable code
    TODO: winRun() does too much — split into buildScene / handleEvents / updateCamera / renderFrame
    BUG:  deltaTime is 0 on frame 1 (initialize last before the loop)
    BUG:  ax, ay uninitialized on first mouse drag — call SDL_GetMouseState(&ax,&ay) before loop
    TODO: glClearColor called every frame needlessly — move before loop
*/

void Window::winRun() {
    glViewport(0, 0, width*8/10, height*8/10);

    int x, y;
    int ax = 0, ay = 0;  // FIX: initialize before loop
    SDL_GetMouseState(&ax, &ay);

    Uint64 last = SDL_GetPerformanceCounter();  // FIX: initialize before loop
    Uint64 now  = last;
    double deltaTime = 0;

    SDL_Event e;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);  // FIX: set once, not every frame

    MD_Camera camera(0, 0, -3, 0, 0, 1);
    MD_Shader  shader(SHADER_DIR "trs_shader.vs",
                      SHADER_DIR "trs_shader.fs");
    MD_Renderer renderer;

    MD_Texture texture(IMG_DIR "texture-background.jpg");
    texture.init();
    texture.load_Image();

    MD_Texture texture2(IMG_DIR "cracked_tx.jpg");
    texture2.init();
    texture2.load_Image();

    // --- scene ---
    MD_Sphere sp1(5, 5);
    MD_Object ob1(&sp1, nullptr, { {30.f,0.f,30.f} });
    renderer.addObject(&ob1);

    MD_Sphere sp(25, 25);
    MD_Object ob(&sp, nullptr, { {0.f,1.f,0.f} });
    renderer.addObject(&ob);

    MD_Quad disque(10, 10);
    MD_Object disque_ob(&disque, &texture, { {0.f,0.f,0.f} });
    renderer.addObject(&disque_ob);

    MD_Quad disque1(10, 10);
    MD_Object disque_ob1(&disque1, &texture2, { {0.f,6.f,0.f} });
    renderer.addObject(&disque_ob1);

    MD_Cylinder cylinder(25);
    MD_Object cylinder_ob(&cylinder, nullptr,
        { {2.f,2.f,0.f}, {0.5f,2.f,0.5f} });
    renderer.addObject(&cylinder_ob);

    MD_Disk circle(40);
    MD_Object circle_ob(&circle, nullptr, { {0.f,4.f,0.f} });
    renderer.addObject(&circle_ob);

    // world-space light — transformed to view space each frame
    const Vec3f lightWorldPos(20.f, 1.f, 1.f);
    const Vec3f lightColor(1.f, 0.f, 1.f);

    while (!stop) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.apply();

        // --- events ---
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    stop = true;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                        stop = true;
                    break;
                case SDL_MOUSEMOTION:
                    SDL_GetMouseState(&x, &y);
                    if (e.motion.state & SDL_BUTTON_LMASK)
                        camera.update(false,false,false,false,false,false,
                                      ax-x, ay-y, 5);
                    ax = x;
                    ay = y;
                    break;
                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                        width  = e.window.data1;
                        height = e.window.data2;
                        glViewport(0, 0, width*8/10, height*8/10);
                    }
                    break;
            }
        }

        // --- timing ---
        now       = SDL_GetPerformanceCounter();
        deltaTime = (double)(now - last) / SDL_GetPerformanceFrequency();
        last      = now;

        // --- camera ---
        const Uint8* ks = SDL_GetKeyboardState(NULL);
        camera.update(ks[SDL_SCANCODE_W], ks[SDL_SCANCODE_S],
                      ks[SDL_SCANCODE_A], ks[SDL_SCANCODE_D],
                      ks[SDL_SCANCODE_SPACE], ks[SDL_SCANCODE_N],
                      0, 0, 4*deltaTime);

        camera.setShader(shader.get_program_id());
        camera.setView();   // uploads worldToView

        Mat4f viewToClip = Mat4f::perspective(toRadians(45.0),
                               (float)width/height, 0.1f, 100.0f);
        shader.setMat4("viewToClip", viewToClip);

        // --- lighting (transform to view space) ---
        Mat4f worldToView  = camera.genLookAt();
        Vec3f lightViewPos = worldToView.project(Vec4f(lightViewPos.x, lightViewPos.y, lightViewPos.z, 1.f));

        shader.setInt  ("uNbLights",          1);
        shader.setVec3 ("uLightPositions[0]", lightViewPos);
        shader.setVec3 ("uLightColors[0]",    lightColor);
        shader.setFloat("uAmbient",           0.5f);
        shader.setInt  ("uSpec",              64);

        renderer.render(shader);
        SDL_GL_SwapWindow(win);
    }
}    


