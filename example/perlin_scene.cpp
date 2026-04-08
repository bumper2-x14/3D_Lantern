#include <fstream>
#include <cassert>  
#include <iostream>  
#include <string>

#include "ray_tracer/RT_Camera.h"
#include "ray_tracer/RT_Renderer.h"
#include "ray_tracer/RT_Lambertian.h"
#include "ray_tracer/RT_Sphere.h"
#include "ray_tracer/RT_Cylinder.h"
#include "ray_tracer/RT_Cone.h"
#include "assets/perlin_texture.h"
#include "ray_tracer/RT_Medium.h"
#include "ray_tracer/RT_DirectionalLight.h"

int main() {
    RT_Camera cam(
        Point3d(0, 0, 2),
        Point3d(0, 0, -1),
        Vec3d(0, 1, 0),
        30.0, 0.0, 10.0
    );

    // Materials
    RT_Lambertian mat_marble     (new PerlinTexture(MARBLE,     8));
    RT_Lambertian mat_wood       (new PerlinTexture(WOOD,      20));
    RT_Lambertian mat_turbulence (new PerlinTexture(TURBULENCE, 5));
    RT_Lambertian mat_fire       (new PerlinTexture(FIRE,       6));
    RT_Lambertian mat_water      (new PerlinTexture(WATER,      6));
    RT_Lambertian mat_warped     (new PerlinTexture(WARPED,    20));
    RT_Lambertian mat_ground     (new PerlinTexture(TERRAIN,   10));

    // Objects
    RT_Sphere ground  (&mat_ground);
    RT_Sphere sphere1 (&mat_marble);
    RT_Sphere sphere2 (&mat_wood);
    RT_Sphere sphere3 (&mat_fire);
    RT_Sphere sphere4 (&mat_turbulence);
    RT_Sphere sphere5 (&mat_water);

    TRSTransformd t;
    t.setTranslation({ 0.0, -100.5, -1.0}); t.setScale({100.0, 100.0, 100.0}); ground  .setTransform(t); t.reset();
    t.setTranslation({-1.2,   -0.2, -1.2}); t.setScale({0.3,   0.3,   0.3  }); sphere1 .setTransform(t); t.reset();
    t.setTranslation({ 0.0,   -0.2, -1.2}); t.setScale({0.3,   0.3,   0.3  }); sphere2 .setTransform(t); t.reset();
    t.setTranslation({ 1.2,   -0.2, -1.2}); t.setScale({0.3,   0.3,   0.3  }); sphere3 .setTransform(t); t.reset();
    t.setTranslation({-0.8,    0.6, -2.0}); t.setScale({0.3,   0.3,   0.3  }); sphere4 .setTransform(t); t.reset();
    t.setTranslation({ 0.8,    0.6, -2.0}); t.setScale({0.3,   0.3,   0.3  }); sphere5 .setTransform(t); t.reset();

    // Cylinders — radius=0.2, height=1.7 (y=-0.5 to 1.2), center_y = -0.5+0.85 = 0.35
    RT_Cylinder cyl1(true, &mat_warped);
    RT_Cylinder cyl2(true, &mat_turbulence);

    t.setTranslation({-1.5, 0.35, -2.5}); t.setScale({0.2, 0.85, 0.2}); cyl1.setTransform(t); t.reset();
    t.setTranslation({ 1.5, 0.35, -2.5}); t.setScale({0.2, 0.85, 0.2}); cyl2.setTransform(t); t.reset();

    // Cones — half_angle=0.35rad, k=tan(0.35)≈0.365, height=1.0
    RT_Cone cone1(true, &mat_marble);
    RT_Cone cone2(true, &mat_wood);

    t.setTranslation({-1.0, 0.5, -1.5}); t.setScale({0.365, 1.0, 0.365}); cone1.setTransform(t); t.reset();
    t.setTranslation({ 1.0, 0.5, -1.5}); t.setScale({0.365, 1.0, 0.365}); cone2.setTransform(t); t.reset();

    RT_Sphere smoke_boundary(&mat_ground); 
    TRSTransformd t_smoke;

    t_smoke.setTranslation({0.0, 0.0, -2.0});
    t_smoke.setScale({10.0, 10.0, 10.0});
    smoke_boundary.setTransform(t_smoke);
    RT_Medium smoke(&smoke_boundary, 0.7, Color(1.0, 1.0, 1.0));

    // Light
    RT_DirectionalLight light(
        normalize(Vec3d(-5.0, 5.0, -1.0)),   // direction
        Color(1.0, 1.0, 1.0),      // white light
        1.5                        // intensity
    );

    // Scene
    RT_ObjectList scene;
    scene.add(&ground);
    scene.add(&sphere1);
    scene.add(&sphere2);
    scene.add(&sphere3);
    scene.add(&sphere4);
    scene.add(&sphere5);
    scene.add(&cyl1);
    scene.add(&cyl2);
    scene.add(&cone1);
    scene.add(&cone2);
    //scene.add(&smoke);

    RT_Renderer renderer(800, 16.0/9.0, 100, 20);
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.p_lights.push_back(&light);
    renderer.setBackground(Color(0.0, 0.0, 0.0));
    renderer.render();
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "perlin_scene.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "perlin_scene.ppm");
    assert(file.good());
    std::cout << "perlin_scene.ppm was generated successfully\n";
    return 0;
}