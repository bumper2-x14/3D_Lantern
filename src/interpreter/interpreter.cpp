#include "interpreter.h"

#include "ray_tracer/RT_Sphere.h"
#include "ray_tracer/RT_Cylinder.h"
#include "ray_tracer/RT_Cone.h"
#include "ray_tracer/RT_Mesh.h"
#include "ray_tracer/RT_Box.h"
#include "ray_tracer/RT_Disk.h"
#include "ray_tracer/RT_Quad.h"
#include "ray_tracer/RT_PointLight.h"
#include "ray_tracer/RT_DirectionalLight.h"

#include "ray_tracer/RT_Lambertian.h"
#include "ray_tracer/RT_Metallic.h"
#include "ray_tracer/RT_Dielectric.h"

#include "assets/color_texture.h"
#include "assets/checker_texture.h"
#include "assets/image_texture.h"
#include "assets/perlin_texture.h"

Interpreter::Interpreter(const std::string& _path) {
    Lexer lex(_path);
    lex.tokenize();
    Parser parser(lex.getTokens());
    scene_des = parser.parse();
}

Interpreter::~Interpreter() {
    delete renderer;
    delete camera;
}

RT_Renderer& Interpreter::makeRayTracer() {
    buildTextures();
    buildMaterials();
    buildObjects();
    buildLights();

    const auto& s = scene_des.settings;
    const auto& c = scene_des.camera;

    renderer = new RT_Renderer(s.width, s.aspectRatio, s.samples, s.depth);

    camera = new RT_Camera(
        Point3d(c.position.x, c.position.y, c.position.z),
        Point3d(c.lookat.x,   c.lookat.y,   c.lookat.z),
        Vec3d  (c.upView.x,   c.upView.y,   c.upView.z),
        c.vfov, c.defocusAngle, c.focusDistance
    );

    renderer->setCamera(camera);
    renderer->setScene(&scene);
    renderer->setBackground(Color(s.background.x, s.background.y, s.background.z));

    return *renderer;
}


// Helpers

void Interpreter::buildTextures() {
    for (const auto& [name, td] : scene_des.textures) {
        Texture* tex = nullptr;
        switch (td.type) {
            case TextureType::SOLIDCOLOR:
                tex = new ColorTexture(Color(td.color.x, td.color.y, td.color.z));
                break;
            case TextureType::CHECKER:
                tex = new CheckerTexture(
                    Color(td.even.x, td.even.y, td.even.z),
                    Color(td.odd.x, td.odd.y, td.odd.z),
                    td.scale
                );
                break;
            case TextureType::IMAGE:
                shared.loadImage(name, std::string(IMG_DIR) + td.file_img);
                tex = new ImageTexture(shared.getImage(name));
                break;
            case TextureType::MARBLE:
                tex = new PerlinTexture(MARBLE, td.scale);
                break;
            case TextureType::WOOD:
                tex = new PerlinTexture(WOOD, td.scale);
                break;
            case TextureType::TURBULENCE:
                tex = new PerlinTexture(TURBULENCE, td.scale);
                break;
            case TextureType::NOISE:
                tex = new PerlinTexture(NOISE, td.scale);
                break;
            case TextureType::WARPED:
                tex = new PerlinTexture(WARPED, td.scale);
                break;
            default:
                std::cerr << "Interpreter: unknown texture type for '" << name << "'\n";
                continue;
        }
        shared.addTexture(name, tex);
    }
}

void Interpreter::buildMaterials() {
    for (const auto& [name, md] : scene_des.materials) {
        RT_Material* mat = nullptr;
        switch (md.type) {
            case MaterialType::LAMBERTIAN:
                if (!md.textureRef.empty())
                    mat = new RT_Lambertian(resolveTexture(md.textureRef));
                else
                    mat = new RT_Lambertian(Color(md.color.x, md.color.y, md.color.z));
                break;
            case MaterialType::METALLIC:
                mat = new RT_Metallic(Color(md.color.x, md.color.y, md.color.z), md.fuzz);
                break;
            case MaterialType::DIELECTRIC:
                mat = new RT_Dielectric(md.ior);
                break;
            default:
                std::cerr << "Interpreter: unknown material type for '" << name << "'\n";
                continue;
        }
        rt.addMaterial(name, mat);
    }
}

void Interpreter::buildObjects() {
    for (const auto& [name, od] : scene_des.objects) {
        RT_Material* mat = resolveMaterial(od.materialRef);
        if (!mat) {
            std::cerr << "Interpreter: missing material '" << od.materialRef
                      << "' for object '" << name << "'\n";
            continue;
        }

        RT_Object* obj = nullptr;
        switch (od.type) {
            case ObjectType::SPHERE:
                obj = scene.emplaceObject<RT_Sphere>(mat); break;
            case ObjectType::CYLINDER:
                obj = scene.emplaceObject<RT_Cylinder>(od.capped, mat); break;
            case ObjectType::CONE:
                obj = scene.emplaceObject<RT_Cone>(od.capped, mat); break;
            case ObjectType::BOX:
                obj = scene.emplaceObject<RT_Box>(mat); break;
            case ObjectType::DISK:
                obj = scene.emplaceObject<RT_Disk>(mat); break;
            case ObjectType::QUAD:
                obj = scene.emplaceObject<RT_Quad>(mat); break;
            case ObjectType::MESH: {
                shared.loadModel(name, std::string(RESOURCE_DIR) + od.file_mesh);
                Model* model = shared.getModel(name);
                if (!model) {
                    std::cerr << "Interpreter: failed to load mesh '"
                            << od.file_mesh << "'\n";
                    continue;
                }
                obj = scene.emplaceObject<RT_Mesh>(model->getMesh(), mat);
                break;
            }

            default:
                std::cerr << "Interpreter: unknown object type for '" << name << "'\n";
                continue;
        }

        TRSTransformd t;
        t.setTranslation(od.translate);
        t.setRotation(od.rotate);
        t.setScale(od.scale);
        obj->setTransform(t);
    }
}


void Interpreter::buildLights() {
    for (const auto& ld : scene_des.lights) {
        switch (ld.type) {
            case LightType::POINT:
                scene.emplaceLight<RT_PointLight>(
                    ld.position,
                    Color(ld.color.x, ld.color.y, ld.color.z),
                    ld.intensity
                );
                break;
            case LightType::DIRECTIONAL:
                scene.emplaceLight<RT_DirectionalLight>(
                    ld.direction,
                    Color(ld.color.x, ld.color.y, ld.color.z),
                    ld.intensity
                );
                break;
            default:
                std::cerr << "Interpreter: unknown light type\n";
                break;
        }
    }
}

Texture* Interpreter::resolveTexture(const std::string& name) const {
    Texture* tex = shared.getTexture(name);
    if (!tex)
        std::cerr << "Interpreter: texture '" << name << "' not found\n";
    return tex;
}

RT_Material* Interpreter::resolveMaterial(const std::string& name) const {
    RT_Material* mat = rt.getMaterial(name);
    if (!mat)
        std::cerr << "Interpreter: material '" << name << "' not found\n";
    return mat;
}

