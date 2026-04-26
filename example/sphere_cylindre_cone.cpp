#include <fstream>
#include <cassert>
#include <iostream>
#include <string>

#include "ray_tracer/RT_Camera.h"
#include "ray_tracer/RT_Renderer.h"
#include "ray_tracer/RT_Sphere.h"
#include "ray_tracer/RT_Cylinder.h"
#include "ray_tracer/RT_Cone.h"
#include "ray_tracer/RT_PointLight.h"

#include "ray_tracer/RT_Lambertian.h"
#include "ray_tracer/RT_Dielectric.h"
#include "ray_tracer/RT_Metallic.h"

#include "assets/checker_texture.h"
#include "assets/image_texture.h"
#include "assets/perlin_texture.h"

#include "assets/shared_resources.h"
#include "ray_tracer/raytracer_resources.h"

int main() {

    // resources
    SharedResources shared;
    RaytracerResources rt;

    // textures
    shared.addTexture("perlin_marble",  new PerlinTexture(PerlinClassType::MARBLE,      10));
    shared.addTexture("perlin_wood",    new PerlinTexture(PerlinClassType::WOOD,        30));
    shared.addTexture("perlin_turb",    new PerlinTexture(PerlinClassType::TURBULENCE,   5));
    shared.addTexture("checker",        new CheckerTexture(Color(1,1,1), Color(0,0,0), 4));
    shared.addTexture("bg_image",       new ImageTexture(IMG_DIR "texture-background.jpg"));

    // materials
    rt.addMaterial("center",     new RT_Metallic  (Color(0.4, 0.4, 0.8), 0.0));
    rt.addMaterial("left",       new RT_Lambertian(shared.getTexture("perlin_marble")));
    rt.addMaterial("right",      new RT_Dielectric(1.490));
    rt.addMaterial("small",      new RT_Metallic  (Color(0.8, 0.8, 0.3), 0.6));
    rt.addMaterial("ground",     new RT_Lambertian(shared.getTexture("checker")));
    rt.addMaterial("cyl_left",   new RT_Metallic  (Color(0.2, 0.8, 0.4), 0.1));
    rt.addMaterial("cyl_right",  new RT_Lambertian(shared.getTexture("bg_image")));
    rt.addMaterial("cyl_lback",  new RT_Lambertian(shared.getTexture("perlin_wood")));
    rt.addMaterial("cone_left",  new RT_Lambertian(shared.getTexture("perlin_turb")));
    rt.addMaterial("cone_right", new RT_Metallic  (Color(0.9, 0.7, 0.2), 0.0));

    // camera
    RT_Camera cam(
        Point3d(0, 0,  1),
        Point3d(0, 0, -1),
        Vec3d  (0, 3,  0),
        60.0, 0.0, 10.0
    );

    RT_Scene scene;
    TRSTransformd t;
    // primitives
    auto* sphere_center = scene.emplaceObject<RT_Sphere>(rt.getMaterial("center"));
    t.setTranslation({ 0.0,  -0.2,  -1.0}); t.setScale({0.3,   0.3,   0.3  }); sphere_center->setTransform(t); t.reset();
    auto* sphere_left = scene.emplaceObject<RT_Sphere>(rt.getMaterial("left"));    
    t.setTranslation({-0.8,  -0.2,  -1.2}); t.setScale({0.3,   0.3,   0.3  }); sphere_left->setTransform(t); t.reset();
    auto* sphere_right = scene.emplaceObject<RT_Sphere>(rt.getMaterial("right")); 
    t.setTranslation({ 0.8,  -0.2,  -1.2}); t.setScale({0.3,   0.3,   0.3  }); sphere_right->setTransform(t); t.reset();
    auto* sphere_small = scene.emplaceObject<RT_Sphere>(rt.getMaterial("small")); 
    t.setTranslation({ 0.0,   0.55, -1.5}); t.setScale({0.25,  0.25,  0.25 }); sphere_small->setTransform(t); t.reset();
    auto* sphere_ground = scene.emplaceObject<RT_Sphere>(rt.getMaterial("ground")); 
    t.setTranslation({ 0.0,-100.5,  -1.0}); t.setScale({100.0, 100.0, 100.0}); sphere_ground->setTransform(t); t.reset();

    auto* cyl_left = scene.emplaceObject<RT_Cylinder>(true, rt.getMaterial("cyl_left")); 
    t.setTranslation({-1.5, -0.05, -1.5}); t.setScale({0.18, 0.95, 0.18}); cyl_left->setTransform(t); t.reset();
    auto* cyl_right = scene.emplaceObject<RT_Cylinder>(true, rt.getMaterial("cyl_right")); 
    t.setTranslation({ 1.5, -0.05, -1.5}); t.setScale({0.18, 0.95, 0.18}); cyl_right->setTransform(t); t.reset();
    auto* cyl_left_back = scene.emplaceObject<RT_Cylinder>(true, rt.getMaterial("cyl_lback")); 
    t.setTranslation({-1.0, -0.05, -2.5}); t.setScale({0.18, 0.95, 0.18}); cyl_left_back->setTransform(t); t.reset();

    auto* cone_left = scene.emplaceObject<RT_Cone>(true, rt.getMaterial("cone_left")); 
    t.setTranslation({-1.5, 0.5, -1.0}); t.setScale({0.364, 1.0, 0.364}); cone_left->setTransform(t); t.reset();
    auto* cone_right = scene.emplaceObject<RT_Cone>(true, rt.getMaterial("cone_right")); 
    t.setTranslation({ 1.5, 0.5, -1.0}); t.setScale({0.364, 1.0, 0.364}); cone_right->setTransform(t); t.reset();

    // light
    RT_PointLight light(Point3d(-5.0, 5.0, -1.0), Color(1.0, 1.0, 1.0), 2.5);
    scene.emplaceLight<RT_PointLight>(Point3d(-5.0, 5.0, -1.0), Color(1.0, 1.0, 1.0), 2.5);

    // render
    RT_Renderer renderer(800, 16.0/9.0, 100, 20);
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.setBackground(Color(0.0, 0.0, 0.0));
    renderer.render(true);
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "sphere_cylindre_cone.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "sphere_cylindre_cone.ppm");
    assert(file.good());
    std::cout << "sphere_cylindre_cone.ppm was generated successfully\n";
}