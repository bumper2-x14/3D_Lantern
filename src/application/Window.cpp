#include "Window.h"
#include "modeling/MD_Camera.h"
#include "modeling/MD_Shader.h"
#include "modeling/MD_Sphere.h"
#include "modeling/MD_Object.h"
#include "math/mat4.h"
#include "math/vec3.h"
#include "math/transform.h"

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

Window::Window(){
    sdlSetAttributes();
    win = SDL_CreateWindow( "window", 100, 100, 1920, 1080,
                SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE |
                                            SDL_WINDOW_OPENGL );
    width = 1920;
    height = 1080;
}


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


void Window::winRun(){
        glViewport(0 ,0 , width*8/10, height*8/10) ;
        int x , y , ax , ay;
        Uint64 now=0;
        Uint64 last=0 ;
        SDL_Event e;
        double deltaTime = 0;
        glEnable(GL_DEPTH_TEST);
        MD_Camera camera(0,0,-3,0,0,1);
        MD_Shader shader("../../../shaders/shader.vs","../../../shaders/shader.fs");
        

        MD_Sphere sp1(5,5);
        MD_Sphere sp(25,25);   
        Transform t1=Transform::translate(Vec3f(30.0,0,30.0)); 
        Transform t=Transform::translate(Vec3f(0,0,0));
        MD_Object ob(&sp,&t);
        MD_Object ob1(&sp1,&t1);

        

        while(!stop){

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shader.apply();
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            while(SDL_PollEvent(&e)){
                    switch(e.type){
                        case SDL_QUIT:
                            stop = true;
                            break;
                        case SDL_KEYDOWN:
                            if(e.key.keysym.sym == SDLK_ESCAPE)
                                stop = true;
                            break;
                        case SDL_MOUSEMOTION:
                            SDL_GetMouseState( &x, &y); 
                            if(e.motion.state & SDL_BUTTON_LMASK ){    
                                camera.update(false,false,false,false,
                                                    ax-x,ay-y,5); 
                            }
                            ax=x; 
                            ay=y;
                            break;
                    case SDL_WINDOWEVENT:
                        if(e.window.event == SDL_WINDOWEVENT_RESIZED){
                            int newWidth = e.window.data1;
                            int newHeight = e.window.data2;
                            width=newWidth;
                            height=newHeight;
                            glViewport(0,0,newWidth*8/10,newHeight*8/10);
                        }
                        break;
                }
        
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        camera.update(
                    keystate[SDL_SCANCODE_W],
                    keystate[SDL_SCANCODE_S], 
                    keystate[SDL_SCANCODE_A], 
                    keystate[SDL_SCANCODE_D], 
                    0,0,4*deltaTime);

        now = SDL_GetPerformanceCounter();
        deltaTime = (double)(now - last) / SDL_GetPerformanceFrequency();
        last = now;
        camera.setShader(shader.get_program_id());
        Mat4f locWor;
        shader.setMat4("localToWorld" , locWor);
        camera.setView();
        //Mat4f worVie;
        //shader.setMat4("worldToView" , worVie);
        Mat4f vieCli;
        vieCli = Mat4f::perspective( toRadians(45.0) , (float)width/height , 0.1f, 100.0f);
        shader.setMat4("viewToClip" , vieCli);

        // NOTE FOR DEV: Draw function was updated to take the shder program's id in order to
        // draw the object wireframe 
            
        ob.draw(shader);    
        ob1.draw(shader);
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES,0,3);
        //all of the code below should be found in renderer class 
        //glClear(GL_COLOR_BUFFER_BIT );
        SDL_GL_SwapWindow(win);
    }
}


//should put a desctuctor 

