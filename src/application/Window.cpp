#include "Window.h"
#include "modeling/MD_Camera.h"
#include "modeling/MD_Shader.h"
#include "modeling/MD_Sphere.h"
#include "modeling/MD_Object.h"


Window::Window(){

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow( "window", 100, 100, 1920, 1080,
                SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE |
                                            SDL_WINDOW_OPENGL );
    width = 1920;
    height = 1080;
}

Window::Window(const std::string &name){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow(name.c_str(), 100, 100, 1920, 1080,
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE |
                                            SDL_WINDOW_OPENGL );
    width = 1920;
    height = 1080;
}

Window::Window(const std::string &name, int _width, int _height){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow(name.c_str(), 100, 100, _width, _height,
                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL );
    width = _width;
    height = _height;
}

Window::Window(const std::string &name, int _width, int _height, int _x, int _y){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
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
    SDL_Event e;
        MD_Camera camera(0,0,-3,0,0,1);
        MD_Shader shader("shader.vs","shader.fs");
        Sphere sp(10,10);   
        //MD_Object ob(&sp,nullptr);
        while(!stop){
           while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                stop = true;
            }
            if(e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_ESCAPE)
                    stop = true;
                if(e.key.keysym.sym==SDLK_w){
                    camera.update(true,false,false,false,0,0,10);
                }
                if(e.key.keysym.sym==SDLK_s){
                    camera.update(false,true,false,false,0,0,10);
                }
                if(e.key.keysym.sym==SDLK_a){
                    camera.update(false,false,true,false,0,0,10);
                }
                if(e.key.keysym.sym==SDLK_d){
                    camera.update(false,false,false,true,0,0,10);
                }
            }
        }
        shader.apply();
        camera.setShader(shader.get_program_id());
        Mat4f locWor;
        shader.setMat4("localToWorld" , locWor);
        camera.setView();
        Mat4f viewToClip;
        viewToClip = Mat4f::perspective( toRadians(90.0d) , (float)width/height , 0.1f, 100.0f);
        //ob.draw();    
        //all of the code below should be found in renderer class 
        

    SDL_GL_SwapWindow(win);
    }
}
//should put a desctuctor 
