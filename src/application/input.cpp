#include "inputs.h"

void Input::update() {
    prev_keys = curr_keys;
    prev_mouse = curr_mouse;

    delta_x = delta_y = 0;

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            
            case SDL_KEYDOWN:
                curr_keys[e.key.keysym.scancode] = true;
                break;
            
            case SDL_KEYUP:
                if (e.button.button < curr_mouse.size())
                    curr_keys[e.key.keysym.scancode] = false;
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button < curr_mouse.size())
                    curr_mouse[e.button.button] = true;
                break;
            
            case SDL_MOUSEMOTION:
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
                delta_x += e.motion.xrel;
                delta_y += e.motion.yrel;
                break;
        }
    }
}

// keyboard
bool Input::isKeyDown(SDL_Scancode key) const {
    return curr_keys[key];
}

bool Input::isKeyPressed(SDL_Scancode key) const {
    return curr_keys[key] && !prev_keys[key];
}

bool Input::isKeyReleased(SDL_Scancode key) const {
    return !curr_keys[key] && prev_keys[key];
}

// mouse buttons
bool Input::isMouseDown(Uint8 button) const {
    return curr_mouse[button];
}

bool Input::isMousePressed(Uint8 button) const {
    return curr_mouse[button] && !prev_mouse[button];
}

bool Input::isMouseReleased(Uint8 button) const {
    return !curr_keys[button] && prev_mouse[button];
}

// mouse position 
int Input::getMouseX() const { return mouse_x; }
int Input::getMouseY() const { return mouse_y; }

// mouse movement 
int Input::getDeltaX() const { return delta_x; }
int Input::getDeltaY() const { return delta_y; }

// system
bool Input::quitPressed() const { return quit; } 