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

struct SerializerSettings {
    int width = 800;
    int height = 450;
    int samples = 100;
    int depth = 20;
    double aspectRatio = 16.0 / 9.0;
    Vec3f background = {0, 0, 0};
};

class SceneSerializer {
public:
    static std::string serialize(const MD_Scene& scene, const MD_Camera& camera, const SharedResources& shared,
                                 const ModelingResources& modeling, const SerializerSettings& settings = SerializerSettings{});

private:
    static std::string v3(const Vec3f& v);
    static std::string f(float v);

    static void writeSettings (std::ostringstream& o, const SerializerSettings& s);
    static void writeCamera (std::ostringstream& o, const MD_Camera& cam);
    static void writeTextures (std::ostringstream& o, const SharedResources& shared);
    static void writeMaterials(std::ostringstream& o, const ModelingResources& modeling, const SharedResources& shared);
    static void writeObjects (std::ostringstream& o, const MD_Scene& scene);
    static void writeLights (std::ostringstream& o, const MD_Scene& scene);
};

#endif