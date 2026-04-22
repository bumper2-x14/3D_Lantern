#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "lexer.h"
#include "ray_tracer/RT_Renderer.h"

class Interpreter {
    public:
        Interpreter(const std::string& _path);
        ~Interpreter();

        Interpreter(const Interpreter&) = delete;
        Interpreter& operator=(const Interpreter&) = delete;

        RT_Renderer& makeRayTracer();

    private:
        SceneDescriptor scene_des;   
};

#endif