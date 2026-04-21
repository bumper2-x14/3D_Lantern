#include <fstream>
#include <cassert>
#include <iostream>
#include "ray_tracer/RT_Camera.h"
#include "ray_tracer/RT_Renderer.h"
#include "ray_tracer/RT_Lambertian.h"
#include "ray_tracer/RT_Metallic.h"
#include "ray_tracer/RT_Sphere.h"
#include "ray_tracer/RT_Quad.h"


int main() {
    RT_Scene scene;

    RT_Camera cam(
        Point3d(0, 2, 5),
        Point3d(0, 0, 0),
        Vec3d(0, 1, 0),
        90.0, 0.0, 1.0
    );

    // Materials
    RT_Lambertian mat_floor  (Color(0.8, 0.8, 0.8));
    RT_Lambertian mat_back   (Color(0.4, 0.6, 0.9));
    RT_Lambertian mat_left   (Color(0.8, 0.2, 0.2));
    RT_Lambertian mat_right  (Color(0.2, 0.8, 0.2));
    RT_Lambertian mat_sphere (Color(0.9, 0.7, 0.2));
    RT_Metallic   mat_mirror (Color(1.0, 1.0, 1.0), 0.0);

    // Quads — canonical unit quad in XY plane, setTransform positions them
    RT_Quad floor_q    (&mat_floor);
    RT_Quad back_wall  (&mat_back);
    RT_Quad left_wall  (&mat_left);
    RT_Quad right_wall (&mat_right);

    TRSTransformd t;

    // Floor: flat at y=0, 6x4 units, centered at (0,0,-2)
    // canonical quad is XY, rotate -90 around X to make it XZ
    t.setTranslation({0.0,  0.0, -2.0});
    t.setRotation   ({-90.0, 0.0, 0.0});
    t.setScale      ({6.0,  4.0,  1.0});
    floor_q.setTransform(t); t.reset();

    // Back wall: at z=-4, 6x4 units
    t.setTranslation({0.0, 2.0, -4.0});
    t.setScale      ({6.0, 4.0,  1.0});
    back_wall.setTransform(t); t.reset();

    // Left wall: at x=-3, 4x4 units, rotate 90 around Y
    t.setTranslation({-3.0, 2.0, -2.0});
    t.setRotation   ({0.0, 90.0, 0.0});
    t.setScale      ({4.0, 4.0,  1.0});
    left_wall.setTransform(t); t.reset();

    // Right wall: at x=3, rotate -90 around Y
    t.setTranslation({3.0, 2.0, -2.0});
    t.setRotation   ({0.0, -90.0, 0.0});
    t.setScale      ({4.0, 4.0,   1.0});
    right_wall.setTransform(t); t.reset();

    // Spheres
    RT_Sphere sphere       (&mat_sphere);
    RT_Sphere mirror_sphere(&mat_mirror);

    t.setTranslation({0.0, 0.5, -2.0}); t.setScale({0.5, 0.5, 0.5}); sphere       .setTransform(t); t.reset();
    t.setTranslation({1.2, 0.4, -1.5}); t.setScale({0.4, 0.4, 0.4}); mirror_sphere.setTransform(t); t.reset();

    // Scene
    scene.addObject(&floor_q);
    scene.addObject(&back_wall);
    scene.addObject(&left_wall);
    scene.addObject(&right_wall);
    scene.addObject(&sphere);
    scene.addObject(&mirror_sphere);

    RT_Renderer renderer(800, 16.0/9.0, 100, 50);
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.setBackground(Color(0.5, 0.7, 1.0));
    renderer.render();
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "planar.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "planar.ppm");
    assert(file.good());
    std::cout << "planar.ppm generated successfully\n";
    return 0;
}