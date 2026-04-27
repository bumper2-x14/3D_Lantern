#ifndef SCENE_DESCRIPTOR_H
#define SCENE_DESCRIPTOR_H

#include <vector>
#include <unordered_map>
#include "keyword_map.h"
#include "math/vec3.h"
#include "math/point3.h"

/// @brief Stores global render settings from the scene file.
struct SettingDescriptor {
    int width = 800, height = 600; ///< Image resolution.
    int samples = 100, depth = 20; ///< Ray tracing samples and max depth.
    double aspectRatio = 1.777; ///< Image aspect ratio.
    Vec3d background = {0, 0, 0}; ///< Background color.

    /// @brief Sets image width.
    SettingDescriptor& setWidth(int v)               { width = v;       return *this; }

    /// @brief Sets image height.
    SettingDescriptor& setHeight(int v)              { height = v;      return *this; }

    /// @brief Sets number of samples.
    SettingDescriptor& setSamples(int v)             { samples = v;     return *this; }

    /// @brief Sets max ray depth.
    SettingDescriptor& setDepth(int v)               { depth = v;       return *this; }

    /// @brief Sets aspect ratio.
    SettingDescriptor& setAspectRatio(double v)      { aspectRatio = v; return *this; }

    /// @brief Sets background color.
    SettingDescriptor& setBackground(const Vec3d& v) { background = v;  return *this; }
};

/// @brief Stores camera data parsed from the scene file.
struct CameraDescriptor {
    Vec3d position = {0,0,0}, lookat = {0,0,0}, upView = {0,1,0}; ///< Camera position and orientation.
    double vfov = 45.0; ///< Vertical field of view.
    double defocusAngle = 0.0; ///< Defocus angle for depth of field.
    double focusDistance = 10.0; ///< Focus distance for depth of field.

    /// @brief Sets camera position.
    CameraDescriptor& setPosition(const Vec3d& v)      { position = v;      return *this; }

    /// @brief Sets camera target point.
    CameraDescriptor& setLookat(const Vec3d& v)        { lookat = v;        return *this; }

    /// @brief Sets camera up vector.
    CameraDescriptor& setUpView(const Vec3d& v)        { upView = v;        return *this; }

    /// @brief Sets vertical field of view.
    CameraDescriptor& setVfov(double v)                { vfov = v;          return *this; }

    /// @brief Sets defocus angle.
    CameraDescriptor& setDefocusAngle(double v)        { defocusAngle = v;  return *this; }

    /// @brief Sets focus distance.
    CameraDescriptor& setFocusDistance(double v)       { focusDistance = v; return *this; }
};

/// @brief Stores one texture description before creating the real Texture object.
struct TextureDescriptor {
    std::string name; ///< Texture name used as an identifier.
    TextureType type; ///< Type of texture.
    // solidcolor / checker / perlin
    Vec3d color, even, odd; ///< Colors used by solid/checker/procedural textures.
    double scale = 1.0; ///< Texture scale.
    // image
    std::string file_img; ///< Image file path.

    /// @brief Sets texture name.
    TextureDescriptor& setName(const std::string& v)   { name = v;     return *this; }

    /// @brief Sets texture type.
    TextureDescriptor& setType(TextureType v)          { type = v;     return *this; }

    /// @brief Sets main color.
    TextureDescriptor& setColor(const Vec3d& v)        { color = v;    return *this; }

    /// @brief Sets even color for checker texture.
    TextureDescriptor& setEven(const Vec3d& v)         { even = v;     return *this; }

    /// @brief Sets odd color for checker texture.
    TextureDescriptor& setOdd(const Vec3d& v)          { odd = v;      return *this; }

    /// @brief Sets texture scale.
    TextureDescriptor& setScale(double v)              { scale = v;    return *this; }

    /// @brief Sets image file path.
    TextureDescriptor& setFile(const std::string& v)   { file_img = v; return *this; }
};

/// @brief Stores one material description before creating the real material.
struct MaterialDescriptor {
    std::string name; ///< Material name used as an identifier.
    MaterialType type; ///< Material type.
    std::string textureRef;   // identifier → links to TextureDescriptor by name
    Vec3d color; ///< Base color.
    double fuzz = 0.0, ior = 1.0, intensity = 1.0; ///< Material parameters.

    /// @brief Sets material name.
    MaterialDescriptor& setName(const std::string& v)        { name = v;       return *this; }

    /// @brief Sets material type.
    MaterialDescriptor& setType(MaterialType v)              { type = v;       return *this; }

    /// @brief Sets texture reference name.
    MaterialDescriptor& setTextureRef(const std::string& v)  { textureRef = v; return *this; }

    /// @brief Sets material color.
    MaterialDescriptor& setColor(const Vec3d& v)             { color = v;      return *this; }

    /// @brief Sets fuzz value for metallic material.
    MaterialDescriptor& setFuzz(double v)                    { fuzz = v;       return *this; }

    /// @brief Sets index of refraction.
    MaterialDescriptor& setIor(double v)                     { ior = v;        return *this; }

    /// @brief Sets light intensity for emissive material.
    MaterialDescriptor& setIntensity(double v)               { intensity = v;  return *this; }
};

/// @brief Stores one object description before creating the real scene object.
struct ObjectDescriptor {
    std::string name; ///< Object name.
    ObjectType  type; ///< Object shape type.
    std::string materialRef; ///< Material reference name.
    std::string file_mesh; // mesh only

    // transforms — object is unit size at origin, these move/size it
    Vec3d translate = {0, 0, 0}; ///< Object translation.
    Vec3d rotate = {0, 0, 0}; ///< Object rotation.
    Vec3d scale = {1, 1, 1}; ///< Object scale.

    bool capped = true; ///< Used for shapes that can be open/closed.

    /// @brief Sets object name.
    ObjectDescriptor& setName(const std::string& v)         { name = v;        return *this; }

    /// @brief Sets object type.
    ObjectDescriptor& setType(ObjectType v)                 { type = v;        return *this; }

    /// @brief Sets material reference.
    ObjectDescriptor& setMaterialRef(const std::string& v)  { materialRef = v; return *this; }

    /// @brief Sets mesh file path.
    ObjectDescriptor& setFile(const std::string& v)         { file_mesh = v;   return *this; }

    /// @brief Sets translation.
    ObjectDescriptor& setTranslate(const Vec3d& v)          { translate = v;   return *this; }

    /// @brief Sets rotation.
    ObjectDescriptor& setRotate(const Vec3d& v)             { rotate = v;      return *this; }

    /// @brief Sets scale.
    ObjectDescriptor& setScale(const Vec3d& v)              { scale = v;       return *this; }

    /// @brief Sets whether the shape is capped.
    ObjectDescriptor& setCapped(bool capped)                { capped = true;   return *this; }
};

/// @brief Stores one light description.
struct LightDescriptor {
    LightType type; ///< Light type.
    Vec3d direction; ///< Direction for directional lights.
    Point3d position; ///< Position for point lights.
    Vec3d color = {1, 1, 1}; ///< Light color.
    double intensity = 1.0; ///< Light intensity.

    /// @brief Sets light type.
    LightDescriptor& setType(LightType v)            { type = v;      return *this; }

    /// @brief Sets light direction.
    LightDescriptor& setDirection(const Vec3d& v)    { direction = v; return *this; }

    /// @brief Sets light position.
    LightDescriptor& setPosition(const Point3d& v)   { position = v;  return *this; }

    /// @brief Sets light color.
    LightDescriptor& setColor(const Vec3d& v)        { color = v;     return *this; }

    /// @brief Sets light intensity.
    LightDescriptor& setIntensity(double v)          { intensity = v; return *this; }
};

/// @brief Stores the full parsed scene before the interpreter builds real objects.
struct SceneDescriptor {
    SettingDescriptor settings; ///< Render settings.
    CameraDescriptor  camera; ///< Camera description.
    std::unordered_map<std::string, TextureDescriptor> textures; ///< Textures by name.
    std::unordered_map<std::string, MaterialDescriptor> materials; ///< Materials by name.
    std::unordered_map<std::string, ObjectDescriptor> objects; ///< Objects by name.
    std::vector<LightDescriptor> lights; ///< Scene lights.

    // lookup helper for the interpreter stage

    /// @brief Finds a texture descriptor by name.
    const TextureDescriptor*  findTexture (const std::string& name) const {
        auto it = textures.find(name);
        return it != textures.end() ? &it->second : nullptr;
    }

    // lookup helper for the interpreter stage

    /// @brief Finds a material descriptor by name.
    const MaterialDescriptor* findMaterial(const std::string& name) const {
        auto it = materials.find(name);
        return it != materials.end() ? &it->second : nullptr;
    }
};

#endif