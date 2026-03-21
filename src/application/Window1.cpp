#include "Window.h"

Window::Window(){

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow( "window", 100, 100, 800, 600,
                SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE |
                                            SDL_WINDOW_OPENGL );
}

Window::Window(const std::string &name){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow(name.c_str(), 100, 100, 800, 600,
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE |
                                            SDL_WINDOW_OPENGL );
}

Window::Window(const std::string &name, int _width, int _height){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow(name.c_str(), 100, 100, _width, _height,
                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL );
}

Window::Window(const std::string &name, int _width, int _height, int _x, int _y){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow(name.c_str(), _x, _y, _width, _height,
                SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL );

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
    while(!stop){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                stop = true;
            }
            if(e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_ESCAPE)
                    stop = true ;
            }
    }
    SDL_GL_SwapWindow(win);
    }
}
//should put a desctuctor 
