#include <iostream>
#include "interpreter/interpreter.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: lantern <scene.lt> <output.ppm>\n";
        return 1;
    }

    Interpreter interp(argv[1]);
    RT_Renderer& renderer = interp.makeRayTracer();
    renderer.render(true);
    renderer.writePPM(argv[2]);

    std::cout << "done: " << argv[2] << '\n';
    return 0;
}