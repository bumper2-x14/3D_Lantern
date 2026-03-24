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
    // Camera pulled back, looking slightly down into the scene
    RT_Camera cam(
        Point3d(0, 2, 5),    // eye: behind and above
        Point3d(0, 0, 0),    // look-at: scene center
        Vec3d(0, 1, 0),
        90.0, 0.0, 1.0       // fov, aperture, focus_dist
    );

    // Materials
    RT_Lambertian mat_floor  (Color(0.8, 0.8, 0.8));  // grey floor
    RT_Lambertian mat_back   (Color(0.4, 0.6, 0.9));  // blue-ish back wall
    RT_Lambertian mat_left   (Color(0.8, 0.2, 0.2));  // red left wall
    RT_Lambertian mat_right  (Color(0.2, 0.8, 0.2));  // green right wall
    RT_Lambertian mat_sphere (Color(0.9, 0.7, 0.2));  // yellow sphere
    RT_Metallic   mat_mirror (Color(1.0, 1.0, 1.0), 0.0); // perfect mirror

    // Floor: flat quad at y = 0
    RT_Quad floor(
        Point3d(-3.0, 0.0, -4.0),  // origin corner
        Vec3d(6.0, 0.0, 0.0),      // u: right
        Vec3d(0.0, 0.0, 4.0),      // v: forward
        &mat_floor
    );

    // Back wall at z = -4
    RT_Quad back_wall(
        Point3d(-3.0, 0.0, -4.0),
        Vec3d(6.0, 0.0,  0.0),
        Vec3d(0.0, 4.0,  0.0),
        &mat_back
    );

    // Left wall at x = -3
    RT_Quad left_wall(
        Point3d(-3.0, 0.0,  0.0),
        Vec3d(0.0, 0.0, -4.0),
        Vec3d(0.0, 4.0,  0.0),
        &mat_left
    );

    // Right wall at x = 3
    RT_Quad right_wall(
        Point3d( 3.0, 0.0, -4.0),
        Vec3d(0.0, 0.0,  4.0),
        Vec3d(0.0, 4.0,  0.0),
        &mat_right
    );

    // A sphere sitting on the floor (center at y=0.5, radius=0.5)
    RT_Sphere sphere(Point3d(0.0, 0.5, -2.0), 0.5, &mat_sphere);

    // A small mirror sphere next to it
    RT_Sphere mirror_sphere(Point3d(1.2, 0.4, -1.5), 0.4, &mat_mirror);

    // Build scene
    RT_ObjectList scene;
    scene.add(&floor);
    scene.add(&back_wall);
    scene.add(&left_wall);
    scene.add(&right_wall);
    scene.add(&sphere);
    scene.add(&mirror_sphere);

    // Render
    RT_Renderer renderer(800, 16.0/9.0, 100, 50);
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.setBackground(Color(0.5, 0.7, 1.0));
    renderer.render();
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "planar.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "planar.ppm");
    assert(file.good());
    std::cout << "planar.ppm generated successfully\n";
}