#include <fstream>
#include <cassert>  
#include <iostream>  
#include <string>
#include "ray_tracer/RT_Camera.h"
#include "ray_tracer/RT_Renderer.h"
#include "ray_tracer/RT_Lambertian.h"
#include "ray_tracer/RT_Dielectric.h"
#include "ray_tracer/RT_Metallic.h"
#include "ray_tracer/RT_Sphere.h"
#include "ray_tracer/RT_Cylinder.h"
#include "ray_tracer/RT_Cone.h"
#include "assets/checker_texture.h"
#include "assets/image_texture.h"
#include "assets/perlin_texture.h"

int main() {
    RT_Camera cam(
        Point3d(0, 0,  1),
        Point3d(0, 0, -1),
        Vec3d(0, 3, 0),
        60.0, 0.0, 10.0
    );

    // Materials
    RT_Metallic   mat_center    (Color(0.4, 0.4, 0.8), 0);
    RT_Lambertian mat_left      (new PerlinTexture(MARBLE, 10));
    RT_Dielectric mat_right     (1.490);
    RT_Metallic   mat_small     (Color(0.8, 0.8, 0.3), 0.6);
    RT_Lambertian mat_ground    (new CheckerTexture(Color(1.0, 1.0, 1.0), Color(0.0, 0.0, 0.0), 4));
    RT_Metallic   mat_cyl_left  (Color(0.2, 0.8, 0.4), 0.1);
    RT_Lambertian mat_cyl_right (new ImageTexture(IMG_DIR "texture-background.jpg"));
    RT_Lambertian mat_cyl_left_back (new PerlinTexture(WOOD, 30));
    RT_Lambertian mat_cone_left (new PerlinTexture(TURBULENCE, 5));
    RT_Metallic   mat_cone_right(Color(0.9, 0.7, 0.2), 0.0);

    // Spheres
    RT_Sphere sphere_center(&mat_center);
    RT_Sphere sphere_left  (&mat_left);
    RT_Sphere sphere_right (&mat_right);
    RT_Sphere sphere_small (&mat_small);
    RT_Sphere sphere_ground(&mat_ground);

    TRSTransformd t;
    t.setTranslation({ 0.0,  -0.2,  -1.0}); t.setScale({0.3,   0.3,   0.3  }); sphere_center.setTransform(t); t.reset();
    t.setTranslation({-0.8,  -0.2,  -1.2}); t.setScale({0.3,   0.3,   0.3  }); sphere_left  .setTransform(t); t.reset();
    t.setTranslation({ 0.8,  -0.2,  -1.2}); t.setScale({0.3,   0.3,   0.3  }); sphere_right .setTransform(t); t.reset();
    t.setTranslation({ 0.0,   0.55, -1.5}); t.setScale({0.25,  0.25,  0.25 }); sphere_small .setTransform(t); t.reset();
    t.setTranslation({ 0.0,-100.5,  -1.0}); t.setScale({100.0, 100.0, 100.0}); sphere_ground.setTransform(t); t.reset();

    RT_Cylinder cyl_left     (true, &mat_cyl_left);
    RT_Cylinder cyl_right    (true, &mat_cyl_right);
    RT_Cylinder cyl_left_back(true, &mat_cyl_left_back);

    t.setTranslation({-1.5, -0.05, -1.5}); t.setScale({0.18, 0.95, 0.18}); cyl_left     .setTransform(t); t.reset();
    t.setTranslation({ 1.5, -0.05, -1.5}); t.setScale({0.18, 0.95, 0.18}); cyl_right    .setTransform(t); t.reset();
    t.setTranslation({-1.0, -0.05, -2.5}); t.setScale({0.18, 0.95, 0.18}); cyl_left_back.setTransform(t); t.reset();

    RT_Cone cone_left (true, &mat_cone_left);
    RT_Cone cone_right(true, &mat_cone_right);

    t.setTranslation({-1.5, 0.5, -1.0}); t.setScale({0.364, 1.0, 0.364}); cone_left .setTransform(t); t.reset();
    t.setTranslation({ 1.5, 0.5, -1.0}); t.setScale({0.364, 1.0, 0.364}); cone_right.setTransform(t); t.reset();

    // Scene
    RT_ObjectList scene2;
    scene2.add(&sphere_ground);
    scene2.add(&sphere_center);
    scene2.add(&sphere_left);
    scene2.add(&sphere_right);
    scene2.add(&sphere_small);
    scene2.add(&cyl_left);
    scene2.add(&cyl_right);
    scene2.add(&cone_left);
    scene2.add(&cone_right);
    scene2.add(&cyl_left_back);

    RT_Renderer renderer3(800, 16.0/9.0, 100, 20);
    renderer3.setCamera(&cam);
    renderer3.setScene(&scene2);
    renderer3.setBackground(Color(0.5, 0.7, 1.0));
    renderer3.render();
    renderer3.writePPM(EXAMPLE_OUTPUT_DIR "sphere_cylindre_cone.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "sphere_cylindre_cone.ppm");
    assert(file.good());
    std::cout << "sphere_cylindre.ppm was generated successfully\n";
}