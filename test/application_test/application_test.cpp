#include "application/Window.h"
#include <string.h>

int main (){
    
    Window window("name", 800, 600, 100, 100) ;
    window.winInitGl();
    window.winRun();

    return 0;
}
