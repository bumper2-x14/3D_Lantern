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
#include "ray_tracer/RT_Quad.h"

int main() {

    RT_Camera cam(
        Point3d(2, 1.0, 3.0),
        Point3d(0, 0, -1),
        Vec3d(0, 1, 0),
        45.0,   // smaller FOV = better view
        0.0,
        1.0     // IMPORTANT: not 10
    );

    RT_Lambertian ground_mat(Color(0.8, 0.8, 0.8));
    RT_Lambertian red_mat   (Color(0.8, 0.3, 0.3));
    RT_Lambertian blue_mat  (Color(0.3, 0.3, 0.8));
    RT_Metallic   metal_mat (Color(0.8, 0.8, 0.8), 0.1);
    RT_Dielectric glass_mat (1.5);

    // Only ONE mirror
    RT_Metallic mirror_mat(Color(1.0, 1.0, 1.0), 0.0);

    // Big ground (instead of mirror floor)
    RT_Quad ground(
        Point3d(-5, -0.5, -5),
        Vec3d(10, 0, 0),
        Vec3d(0, 0, 10),
        &mirror_mat
    );

    // Back wall (diffuse)
    RT_Quad back_wall(
        Point3d(-5, -0.5, -5),
        Vec3d(10, 0, 0),
        Vec3d(0, 5, 0),
        &blue_mat
    );

    // ONE mirror (right wall)
    RT_Quad mirror_wall(
        Point3d(3, -0.5, -5),
        Vec3d(0, 0, 10),
        Vec3d(0, 5, 0),
        &mirror_mat
    );


    RT_Sphere sphere_center(Point3d(0, 0, -1), 0.5, &metal_mat);
    RT_Sphere sphere_left  (Point3d(-1.2, 0, -1.5), 0.5, &red_mat);
    RT_Sphere sphere_right (Point3d( 1.2, 0, -1.5), 0.5, &glass_mat);

    RT_Cylinder cylinder(
        Point3d(-2.5, -0.5, -2),
        0.3,
        -0.5,
        1.0,
        true,
        &blue_mat
    );

    RT_Cone cone(
        Point3d(2.0, 0.5, -2),
        0.5,
        -1.0,
        0.0,
        true,
        &red_mat
    );

    RT_ObjectList scene;

    scene.add(&ground);
    scene.add(&back_wall);
    scene.add(&mirror_wall);

    scene.add(&sphere_center);
    scene.add(&sphere_left);
    scene.add(&sphere_right);
    scene.add(&cylinder);
    scene.add(&cone);

    // =========================
    // RENDERER
    // =========================
    RT_Renderer renderer(800, 16.0/9.0, 50, 10);

    renderer.setCamera(&cam);
    renderer.setScene(&scene);

    renderer.setBackground(Color(0.5, 0.7, 1.0));

    renderer.render();

    renderer.writePPM(EXAMPLE_OUTPUT_DIR "mirror.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "mirror.ppm");
    assert(file.good());

    std::cout << "clean_scene.ppm generated successfully\n";
}