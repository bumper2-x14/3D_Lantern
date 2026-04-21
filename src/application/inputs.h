#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <array>

class Input {
    public:
        // keyboard
        bool isKeyDown(SDL_Scancode key) const;
        bool isKeyPressed(SDL_Scancode key) const;
        bool isKeyReleased(SDL_Scancode key) const;

        // mouse buttons
        bool isMouseDown(Uint8 button) const;
        bool isMousePressed(Uint8 button) const;
        bool isMouseReleased(Uint8 button) const;

        // mouse position 
        int getMouseX() const;
        int getMouseY() const;

        // mouse movement 
        int getDeltaX() const;
        int getDeltaY() const;

        // system
        bool quitPressed() const;   

        void beginFrame();
        void handleEvent(const SDL_Event& e);
        
    private:
        static constexpr size_t KEY_COUNT = SDL_NUM_SCANCODES;
        std::array<bool, KEY_COUNT> curr_keys{};
        std::array<bool, KEY_COUNT> prev_keys{};
        std::array<bool, 5> curr_mouse{};
        std::array<bool, 5> prev_mouse{};

        int mouse_x = 0 , mouse_y = 0;
        int delta_x = 0 , delta_y = 0;

        bool quit = false;
};

#endif