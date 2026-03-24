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
    //glViewport(0, 0, width, height);
}

Window::Window(const std::string& name){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow(name.c_str(), 100, 100, 1920, 1080,
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE |
                                            SDL_WINDOW_OPENGL );
    width = 1920;
    height = 1080;
    //glViewport(0, 0, width, height);
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
    //glViewport(0, 0, width, height);
}
Window::Window(const char* t, int _width, int _height){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow(t, 100, 100, _width, _height,
                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL );
    width = _width;
    height = _height;
    //glViewport(0, 0, width, height);
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
    //glViewport(0, 0, width, height);
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
        int x , y , ax , ay;
        Uint64 now , last;
        last = SDL_GetPerformanceCounter();
        SDL_Event e;
        double deltaTime = 0;
        glEnable(GL_DEPTH_TEST);
        MD_Camera camera(0,0,-3,0,0,1);
        MD_Shader shader("../../../shaders/shader.vs","../../../shaders/shader.fs");
        MD_Sphere sp(25,25);   
        MD_Object ob(&sp,nullptr);
        while(!stop){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shader.apply();
            while(SDL_PollEvent(&e)){
                if(e.type == SDL_QUIT){
                    stop = true;
                }
                const Uint8* keystate = SDL_GetKeyboardState(NULL);

                camera.update(
                    keystate[SDL_SCANCODE_Z],
                    keystate[SDL_SCANCODE_S],
                    keystate[SDL_SCANCODE_Q],
                    keystate[SDL_SCANCODE_D],
                    0,0,
                    5.0 * deltaTime
                );
                    /*
                    if(e.key.keysym.sym == SDLK_z){
                    camera.update(true,false,false,false,0,0,7*deltaTime);
                    }
                    if(e.key.keysym.sym == SDLK_s){
                    camera.update(false,true,false,false,0,0,7*deltaTime);
                    }
                    if(e.key.keysym.sym == SDLK_q){
                    camera.update(false,false,true,false,0,0,7*deltaTime);
                    }
                    if(e.key.keysym.sym == SDLK_d){
                    camera.update(false,false,false,true,0,0,7*deltaTime);
                    } */
                    /*
                    if(e.type == SDL_MOUSEMOTION ){
                if(e.button.button == SDL_BUTTON_LEFT ){    
                    SDL_GetMouseState( &x, &y); 
                    camera.update(false,false,false,false,x-ax,y-ay,7); 
                    ax=x; 
                    ay=y;
                    */ 
            }
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
            
        ob.draw(shader.get_program_id());    
        
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES,0,3);
        //all of the code below should be found in renderer class 
        //glClear(GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow(win);
    }
}

//should put a desctuctor 
