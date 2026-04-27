#ifndef SCENE_DESCRIPTOR_LOGGER_H
#define SCENE_DESCRIPTOR_LOGGER_H

#include <ostream>
#include <iomanip>
#include "scene_descriptor.h"
#include "keyword_map.h"

// ── helpers ──────────────────────────────────────────────────────────────────

/// @brief Adds indentation spaces to make the log easier to read.
static inline std::ostream& indent(std::ostream& os, int n) {
    for (int i = 0; i < n; ++i) os << "  ";
    return os;
}

/// @brief Prints a Vec3d in a simple readable format.
static inline void logVec3(std::ostream& os, const Vec3d& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

// ── per-descriptor loggers ────────────────────────────────────────────────────

/// @brief Logs the render/settings part of the scene descriptor.
static inline void logSettings(std::ostream& os, const SettingDescriptor& s, int d = 1) {
    indent(os, d) << "[Settings]\n";
    indent(os, d+1) << "Resolution  : " << s.width << " x " << s.height << "\n";
    indent(os, d+1) << "Aspect Ratio: " << s.aspectRatio << "\n";
    indent(os, d+1) << "Samples     : " << s.samples << "\n";
    indent(os, d+1) << "Max Depth   : " << s.depth << "\n";
    indent(os, d+1) << "Background  : "; logVec3(os, s.background); os << "\n";
}

/// @brief Logs camera values from the scene descriptor.
static inline void logCamera(std::ostream& os, const CameraDescriptor& c, int d = 1) {
    indent(os, d) << "[Camera]\n";
    indent(os, d+1) << "Position      : "; logVec3(os, c.position);  os << "\n";
    indent(os, d+1) << "Look At       : "; logVec3(os, c.lookat);    os << "\n";
    indent(os, d+1) << "Up Vector     : "; logVec3(os, c.upView);    os << "\n";
    indent(os, d+1) << "VFov          : " << c.vfov          << " deg\n";
    indent(os, d+1) << "Defocus Angle : " << c.defocusAngle  << " deg\n";
    indent(os, d+1) << "Focus Distance: " << c.focusDistance << "\n";
}

/// @brief Logs one texture descriptor.
static inline void logTexture(std::ostream& os, const TextureDescriptor& t, int d = 1) {
    indent(os, d) << "[Texture] \"" << t.name << "\"\n";
    indent(os, d+1) << "Type  : " << logString(t.type) << "\n";
    indent(os, d+1) << "Color : "; logVec3(os, t.color); os << "\n";
    indent(os, d+1) << "Even  : "; logVec3(os, t.even);  os << "\n";
    indent(os, d+1) << "Odd   : "; logVec3(os, t.odd);   os << "\n";
    indent(os, d+1) << "Scale : " << t.scale << "\n";
    if (!t.file_img.empty())
        indent(os, d+1) << "File  : \"" << t.file_img << "\"\n";
}

/// @brief Logs one material descriptor.
static inline void logMaterial(std::ostream& os, const MaterialDescriptor& m, int d = 1) {
    indent(os, d) << "[Material] \"" << m.name << "\"\n";
    indent(os, d+1) << "Type        : " << logString(m.type) << "\n";
    indent(os, d+1) << "Color       : "; logVec3(os, m.color); os << "\n";
    if (!m.textureRef.empty())
        indent(os, d+1) << "Texture Ref : \"" << m.textureRef << "\"\n";
    indent(os, d+1) << "Fuzz        : " << m.fuzz      << "\n";
    indent(os, d+1) << "IOR         : " << m.ior       << "\n";
    indent(os, d+1) << "Intensity   : " << m.intensity << "\n";
}

/// @brief Logs one object descriptor.
static inline void logObject(std::ostream& os, const ObjectDescriptor& o, int d = 1) {
    indent(os, d) << "[Object] \"" << o.name << "\"\n";
    indent(os, d+1) << "Type         : " << logString(o.type) << "\n";
    indent(os, d+1) << "Material Ref : \"" << o.materialRef << "\"\n";
    if (!o.file_mesh.empty())
        indent(os, d+1) << "Mesh File    : \"" << o.file_mesh << "\"\n";
    indent(os, d+1) << "Translate    : "; logVec3(os, o.translate); os << "\n";
    indent(os, d+1) << "Rotate       : "; logVec3(os, o.rotate);    os << "\n";
    indent(os, d+1) << "Scale        : "; logVec3(os, o.scale);     os << "\n";
}

/// @brief Logs one light descriptor.
static inline void logLight(std::ostream& os, const LightDescriptor& l, int idx, int d = 1) {
    indent(os, d) << "[Light #" << idx << "]\n";
    indent(os, d+1) << "Type      : " << logString(l.type) << "\n";
    indent(os, d+1) << "Color     : "; logVec3(os, l.color);     os << "\n";
    indent(os, d+1) << "Intensity : " << l.intensity << "\n";
    indent(os, d+1) << "Direction : "; logVec3(os, l.direction); os << "\n";
    indent(os, d+1) << "Position  : ("
                    << l.position.x << ", "
                    << l.position.y << ", "
                    << l.position.z << ")\n";
}

// ── top-level scene logger ────────────────────────────────────────────────────

/// @brief Logs the full scene descriptor in a readable way.
inline void logSceneDescriptor(std::ostream& os, const SceneDescriptor& scene) {
    os << "╔══════════════════════════════════════╗\n"
       << "║        Scene Descriptor Dump         ║\n"
       << "╚══════════════════════════════════════╝\n";

    logSettings(os, scene.settings);
    os << "\n";

    logCamera(os, scene.camera);
    os << "\n";

    if (!scene.textures.empty()) {
        os << "  [Textures] (" << scene.textures.size() << ")\n";
        for (const auto& [name, tex] : scene.textures)
            logTexture(os, tex, 2);
        os << "\n";
    }

    if (!scene.materials.empty()) {
        os << "  [Materials] (" << scene.materials.size() << ")\n";
        for (const auto& [name, mat] : scene.materials)
            logMaterial(os, mat, 2);
        os << "\n";
    }

    if (!scene.objects.empty()) {
        os << "  [Objects] (" << scene.objects.size() << ")\n";
        for (const auto& [name, obj] : scene.objects)
            logObject(os, obj, 2);
        os << "\n";
    }

    if (!scene.lights.empty()) {
        os << "  [Lights] (" << scene.lights.size() << ")\n";
        for (int i = 0; i < (int)scene.lights.size(); ++i)
            logLight(os, scene.lights[i], i, 2);
        os << "\n";
    }

    os << "══════════════════════════════════════════\n";
}

#endif