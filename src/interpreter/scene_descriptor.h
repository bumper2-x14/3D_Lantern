#ifndef SCENE_DESCRIPTOR_H
#define SCENE_DESCRIPTOR_H

#include <vector>
#include <unordered_map>
#include "keyword_map.h"
#include "math/vec3.h"
#include "math/point3.h"

struct SettingDescriptor {
    int width = 800, height = 600;
    int samples = 100, depth = 20;
    double aspectRatio = 1.777;
    Vec3d background = {0, 0, 0};

    SettingDescriptor& setWidth(int v)               { width = v;       return *this; }
    SettingDescriptor& setHeight(int v)              { height = v;      return *this; }
    SettingDescriptor& setSamples(int v)             { samples = v;     return *this; }
    SettingDescriptor& setDepth(int v)               { depth = v;       return *this; }
    SettingDescriptor& setAspectRatio(double v)      { aspectRatio = v; return *this; }
    SettingDescriptor& setBackground(const Vec3d& v) { background = v;  return *this; }
};

struct CameraDescriptor {
    Vec3d position = {0,0,0}, lookat = {0,0,0}, upView = {0,1,0};
    double vfov = 45.0;
    double defocusAngle = 0.0;
    double focusDistance = 10.0;

    CameraDescriptor& setPosition(const Vec3d& v)      { position = v;      return *this; }
    CameraDescriptor& setLookat(const Vec3d& v)        { lookat = v;        return *this; }
    CameraDescriptor& setUpView(const Vec3d& v)        { upView = v;        return *this; }
    CameraDescriptor& setVfov(double v)                { vfov = v;          return *this; }
    CameraDescriptor& setDefocusAngle(double v)        { defocusAngle = v;  return *this; }
    CameraDescriptor& setFocusDistance(double v)       { focusDistance = v; return *this; }
};

struct TextureDescriptor {
    std::string name;
    TextureType type;
    // solidcolor / checker / perlin
    Vec3d color, even, odd;
    double scale = 1.0;
    // image
    std::string file_img;

    TextureDescriptor& setName(const std::string& v)   { name = v;     return *this; }
    TextureDescriptor& setType(TextureType v)          { type = v;     return *this; }
    TextureDescriptor& setColor(const Vec3d& v)        { color = v;    return *this; }
    TextureDescriptor& setEven(const Vec3d& v)         { even = v;     return *this; }
    TextureDescriptor& setOdd(const Vec3d& v)          { odd = v;      return *this; }
    TextureDescriptor& setScale(double v)              { scale = v;    return *this; }
    TextureDescriptor& setFile(const std::string& v)   { file_img = v; return *this; }
};

struct MaterialDescriptor {
    std::string name;
    MaterialType type;
    std::string textureRef;   // identifier → links to TextureDescriptor by name
    Vec3d color;
    double fuzz = 0.0, ior = 1.0, intensity = 1.0;

    MaterialDescriptor& setName(const std::string& v)        { name = v;       return *this; }
    MaterialDescriptor& setType(MaterialType v)              { type = v;       return *this; }
    MaterialDescriptor& setTextureRef(const std::string& v)  { textureRef = v; return *this; }
    MaterialDescriptor& setColor(const Vec3d& v)             { color = v;      return *this; }
    MaterialDescriptor& setFuzz(double v)                    { fuzz = v;       return *this; }
    MaterialDescriptor& setIor(double v)                     { ior = v;        return *this; }
    MaterialDescriptor& setIntensity(double v)               { intensity = v;  return *this; }
};

struct ObjectDescriptor {
    std::string name;
    ObjectType  type;
    std::string materialRef;
    std::string file_mesh; // mesh only

    // transforms — object is unit size at origin, these move/size it
    Vec3d translate = {0, 0, 0};
    Vec3d rotate = {0, 0, 0};
    Vec3d scale = {1, 1, 1};

    bool capped = true;

    ObjectDescriptor& setName(const std::string& v)         { name = v;        return *this; }
    ObjectDescriptor& setType(ObjectType v)                 { type = v;        return *this; }
    ObjectDescriptor& setMaterialRef(const std::string& v)  { materialRef = v; return *this; }
    ObjectDescriptor& setFile(const std::string& v)         { file_mesh = v;   return *this; }
    ObjectDescriptor& setTranslate(const Vec3d& v)          { translate = v;   return *this; }
    ObjectDescriptor& setRotate(const Vec3d& v)             { rotate = v;      return *this; }
    ObjectDescriptor& setScale(const Vec3d& v)              { scale = v;       return *this; }
    ObjectDescriptor& setCapped(bool capped)                { capped = true;   return *this; }
};

struct LightDescriptor {
    LightType type;
    Vec3d direction;
    Point3d position;
    Vec3d color = {1, 1, 1};
    double intensity = 1.0;

    LightDescriptor& setType(LightType v)            { type = v;      return *this; }
    LightDescriptor& setDirection(const Vec3d& v)    { direction = v; return *this; }
    LightDescriptor& setPosition(const Point3d& v)   { position = v;  return *this; }
    LightDescriptor& setColor(const Vec3d& v)        { color = v;     return *this; }
    LightDescriptor& setIntensity(double v)          { intensity = v; return *this; }
};

struct SceneDescriptor {
    SettingDescriptor settings;
    CameraDescriptor  camera;
    std::unordered_map<std::string, TextureDescriptor> textures;
    std::unordered_map<std::string, MaterialDescriptor> materials;
    std::unordered_map<std::string, ObjectDescriptor> objects;
    std::vector<LightDescriptor> lights;

    // lookup helper for the interpreter stage
    const TextureDescriptor*  findTexture (const std::string& name) const {
        auto it = textures.find(name);
        return it != textures.end() ? &it->second : nullptr;
    }

    // lookup helper for the interpreter stage
    const MaterialDescriptor* findMaterial(const std::string& name) const {
        auto it = materials.find(name);
        return it != materials.end() ? &it->second : nullptr;
    }
};


#endif