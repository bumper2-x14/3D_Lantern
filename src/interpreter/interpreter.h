#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "lexer.h"
#include "ray_tracer/RT_Renderer.h"
#include "assets/texture.h"
#include "assets/shared_resources.h"
#include "ray_tracer/raytracer_resources.h"

/// @brief Takes an input (file or string), parses it and builds a ray tracing scene.
class Interpreter {
    public:
        /// @brief Loads and interprets a scene from a file.
        /// @param _path Path to scene file.
        Interpreter(const std::string& _path);

        /// @brief Interprets a scene directly from a string.
        /// @param input Scene description.
        /// @param from_string Just to distinguish this constructor.
        Interpreter(const std::string& input, bool from_string);

        /// @brief Destructor (cleans renderer and camera if needed).
        ~Interpreter();

        // disable copy
        Interpreter(const Interpreter&) = delete;
        Interpreter& operator=(const Interpreter&) = delete;

        /// @brief Builds and returns the ray tracer.
        RT_Renderer& makeRayTracer();

    private:
        /// @brief Builds all textures defined in the scene.
        void buildTextures();

        /// @brief Builds all materials using textures.
        void buildMaterials();

        /// @brief Builds all objects (geometry + materials).
        void buildObjects();

        /// @brief Builds all lights.
        void buildLights();

        /// @brief Finds a texture by name.
        Texture* resolveTexture (const std::string& name) const;

        /// @brief Finds a material by name.
        RT_Material* resolveMaterial(const std::string& name) const;

        SceneDescriptor scene_des; ///< Parsed scene description.
        SharedResources shared; ///< Shared resources (images, textures, models).
        RaytracerResources rt; ///< Ray tracer specific resources.
        RT_Scene scene; ///< Final ray tracing scene.
        RT_Renderer* renderer = nullptr;   ///< Renderer instance.
        RT_Camera* camera = nullptr; ///< Camera used for ray tracing.
};

#endif