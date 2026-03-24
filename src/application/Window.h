#ifndef WINDOW_H
#define WINDOW_H

#ifdef __APPLE__
  #define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>


class Window{
	public:
        //constructors
        Window();
        Window(const std::string &name);
        Window(const std::string &name, int _width, int _height);
        Window(const std::string &name, int _width, int _height,
                                            int _x, int _y);
	    Window(const char* t , int x , int y);
        void winInitGl();
        void winRun();
        
        //getter
        SDL_Window* getWin()const;
        int getHeight()const;
        int getWidth()const;
        int getX()const;
        int getY()const;
             

    private:
		void sdlSetAttributes();
        SDL_Window* win=nullptr;
        SDL_GLContext gl_context;
        int height;
        int width;
        int x;
        int y;
        bool stop=false;
};



#endif

