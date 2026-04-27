#include "scene_serializer.h"

std::string SceneSerializer::serialize(const MD_Scene&           scene,
                                       const MD_Camera&          camera,
                                       const SharedResources&    shared,
                                       const ModelingResources&  modeling,
                                       const SerializerSettings& settings) {
    std::ostringstream out;
    writeSettings (out, settings);
    writeCamera   (out, camera);
    writeTextures (out, shared);
    writeMaterials(out, modeling, shared);
    writeObjects  (out, scene);
    writeLights   (out, scene);
    return out.str();
}

// helpers

std::string SceneSerializer::v3(const Vec3f& v) {
    std::ostringstream s;
    s << std::fixed << std::setprecision(4)
      << v.x << " " << v.y << " " << v.z;
    return s.str();
}

std::string SceneSerializer::f(float v) {
    std::ostringstream s;
    s << std::fixed << std::setprecision(4) << v;
    return s.str();
}

// blocks

static Vec3f toRTCoords(const Vec3f& v) {
    return Vec3f(v.x, v.z, -v.y);
}

void SceneSerializer::writeSettings(std::ostringstream& o,
                                    const SerializerSettings& s) {
    o << "setting {\n"
      << "    width "        << s.width   << "\n"
      << "    height "       << s.height  << "\n"
      << "    samples "      << s.samples << "\n"
      << "    depth "        << s.depth   << "\n"
      << "    aspect_ratio " << std::fixed << std::setprecision(4)
                             << s.aspectRatio << "\n"
      << "    background "   << v3(s.background) << "\n"
      << "}\n\n";
}

void SceneSerializer::writeCamera(std::ostringstream& o, const MD_Camera& cam) {
    Vec3f pos    = cam.getPosition();
    Vec3f front  = cam.getFront();
    Vec3f lookat = Vec3f(pos.x + front.x, pos.y + front.y, pos.z + front.z);
    Vec3f up     = cam.getUp();

    o << "camera {\n"
      << "    position "       << v3(toRTCoords(pos))     << "\n"
      << "    lookat "         << v3(toRTCoords(lookat))      << "\n"
      << "    up_view "        << v3(toRTCoords(up))          << "\n"
      << "    vfov "           << f(cam.getFov()) << "\n"
      << "    defocus_angle "  << "0.0\n"
      << "    focus_distance " << "10.0\n"
      << "}\n\n";
}

void SceneSerializer::writeTextures(std::ostringstream&   o,
                                    const SharedResources& shared) {
    for (const auto& [name, tex] : shared.getTextures()) {
        if (!tex) continue;
        std::string body = tex->serializeLNT();
        if (body.empty()) continue;
        o << "texture {\n"
          << "    name " << name << "\n"
          << body
          << "}\n\n";
    }
}

void SceneSerializer::writeMaterials(std::ostringstream&      o,
                                     const ModelingResources& modeling,
                                     const SharedResources&   shared) {
    for (const auto& [name, mat] : modeling.getMaterials()) {
        if (!mat) continue;
        o << "material {\n"
          << "    name " << name << "\n";

        switch (mat->getType()) {
            case MD_Material::MatType::TEXTURE: {
                std::string tex_name = shared.findTextureName(mat->getTexture());
                if (!tex_name.empty())
                    o << "    type lambertian\n"
                      << "    texture_ref " << tex_name << "\n";
                else
                    o << "    type lambertian\n"
                      << "    color 0.8 0.8 0.8\n";
                break;
            }
            case MD_Material::MatType::AMBIENT:
            case MD_Material::MatType::DIFFUSE: {
                Color c = mat->getColor();
                o << "    type lambertian\n"
                  << "    color " << c.r << " " << c.g << " " << c.b << "\n";
                break;
            }
            case MD_Material::MatType::SPECULAR: {
                float fuzz = 1.0f - std::min(mat->getShininess() / 128.f, 1.f);
                Color c = mat->getColor();
                o << "    type metallic\n"
                  << "    color " << c.r << " " << c.g << " " << c.b << "\n"
                  << "    fuzz "  << f(fuzz) << "\n";
                break;
            }
        }
        o << "}\n\n";
    }
}

void SceneSerializer::writeObjects(std::ostringstream& o, const MD_Scene& scene) {
    for (const auto* obj : scene.objects)
        if (obj && obj->getShape())
            o << obj->serialize() << "\n";
}

void SceneSerializer::writeLights(std::ostringstream& o, const MD_Scene& scene) {
    for (const auto* light : scene.point_lights) {
        if (!light) continue;
        o << "light {\n"
          << "    type point\n"
          << "    position "  << v3(toRTCoords(light->getPosition())) << "\n"
          << "    color "     << v3(Vec3f(light->getColor().r,
                                          light->getColor().g,
                                          light->getColor().b)) << "\n"
          << "    intensity " << f(light->getIntensity()) << "\n"
          << "}\n\n";
    }
}

