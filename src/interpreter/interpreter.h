#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "lexer.h"
#include "ray_tracer/RT_Renderer.h"
#include "assets/texture.h"
#include "assets/shared_resources.h"
#include "ray_tracer/raytracer_resources.h"

class Interpreter {
    public:
        Interpreter(const std::string& _path);
        ~Interpreter();

        Interpreter(const Interpreter&) = delete;
        Interpreter& operator=(const Interpreter&) = delete;

        RT_Renderer& makeRayTracer();

    private:
        void buildTextures();
        void buildMaterials();
        void buildObjects();
        void buildLights();

        Texture* resolveTexture (const std::string& name) const;
        RT_Material* resolveMaterial(const std::string& name) const;

        SceneDescriptor scene_des;
        SharedResources shared;
        RaytracerResources rt;
        RT_Scene scene;
        RT_Renderer* renderer = nullptr;   
        RT_Camera* camera = nullptr;
};

#endif