#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include <string>
#include <sstream>
#include <iomanip>

#include "modeling/MD_Scene.h"
#include "modeling/MD_Camera.h"
#include "modeling/MD_Material.h"
#include "modeling/MD_PointLight.h"
#include "assets/shared_resources.h"
#include "modeling/modeling_resources.h"
#include "math/vec3.h"

/// @brief Settings used when exporting the scene.
struct SerializerSettings {
    int width = 800; ///< Output image width.
    int height = 450; ///< Output image height.
    int samples = 100; ///< Number of samples for the render.
    int depth = 20; ///< Max ray depth.
    double aspectRatio = 16.0 / 9.0; ///< Image aspect ratio.
    Vec3f background = {0, 0, 0}; ///< Background color.
    bool threaded = false; ///< True if threaded rendering should be used.
};

/// @brief Turns the current modeling scene into a text scene description.
class SceneSerializer {
public:
    /// @brief Serializes scene data, camera, resources and settings into one string.
    static std::string serialize(const MD_Scene& scene, const MD_Camera& camera, const SharedResources& shared,
                                 const ModelingResources& modeling, const SerializerSettings& settings = SerializerSettings{});

private:
    /// @brief Converts a Vec3f to a string.
    static std::string v3(const Vec3f& v);

    /// @brief Converts a float to a formatted string.
    static std::string f(float v);

    /// @brief Writes the settings block.
    static void writeSettings (std::ostringstream& o, const SerializerSettings& s);

    /// @brief Writes the camera block.
    static void writeCamera (std::ostringstream& o, const MD_Camera& cam);

    /// @brief Writes all texture blocks.
    static void writeTextures (std::ostringstream& o, const SharedResources& shared);

    /// @brief Writes all material blocks.
    static void writeMaterials(std::ostringstream& o, const ModelingResources& modeling, const SharedResources& shared);

    /// @brief Writes all object blocks.
    static void writeObjects (std::ostringstream& o, const MD_Scene& scene);

    /// @brief Writes all light blocks.
    static void writeLights (std::ostringstream& o, const MD_Scene& scene);
};

#endif