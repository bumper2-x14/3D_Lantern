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
#include "ray_tracer/RT_PointLight.h"

int main() { 
    /*
    RT_Scene scene;

    RT_Camera cam(
        Point3d(2, 1.0, 3.0),
        Point3d(0, 0, -1),
        Vec3d(0, 1, 0),
        45.0, 0.0, 1.0
    );

    // Materials
    RT_Lambertian ground_mat(Color(0.8, 0.8, 0.8));
    RT_Lambertian red_mat   (Color(0.8, 0.3, 0.3));
    RT_Lambertian blue_mat  (Color(0.3, 0.3, 0.8));
    RT_Metallic   metal_mat (Color(0.8, 0.8, 0.8), 0.1);
    RT_Dielectric glass_mat (1.5);
    RT_Metallic   mirror_mat(Color(1.0, 1.0, 1.0), 0.0);

    // Quads
    RT_Quad ground     (&mirror_mat);
    RT_Quad back_wall  (&blue_mat);
    RT_Quad mirror_wall(&mirror_mat);

    TRSTransformd t;

    // Ground: flat XZ plane at y=-0.5, 10x10
    t.setTranslation({0.0, -0.5, 0.0});
    t.setRotation   ({-90.0, 0.0, 0.0});
    t.setScale      ({10.0, 10.0, 1.0});
    ground.setTransform(t); t.reset();

    // Back wall: at z=-5, 10 wide x 5 tall, centered at x=0 y=2
    t.setTranslation({0.0, 2.0, -5.0});
    t.setScale      ({10.0, 5.0, 1.0});
    back_wall.setTransform(t); t.reset();

    // Mirror wall: at x=3, 10 deep x 5 tall, rotated 90 around Y
    t.setTranslation({3.0, 2.0, 0.0});
    t.setRotation   ({0.0, 90.0, 0.0});
    t.setScale      ({10.0, 5.0, 1.0});
    mirror_wall.setTransform(t); t.reset();

    // Spheres
    RT_Sphere sphere_center(&metal_mat);
    RT_Sphere sphere_left  (&red_mat);
    RT_Sphere sphere_right (&glass_mat);

    t.setTranslation({ 0.0, 0.0, -1.0}); t.setScale({0.5, 0.5, 0.5}); sphere_center.setTransform(t); t.reset();
    t.setTranslation({-1.2, 0.0, -1.5}); t.setScale({0.5, 0.5, 0.5}); sphere_left  .setTransform(t); t.reset();
    t.setTranslation({ 1.2, 0.0, -1.5}); t.setScale({0.5, 0.5, 0.5}); sphere_right .setTransform(t); t.reset();

    // Cylinder — radius=0.3, height=1.5 (y=-0.5 to 1.0), center_y=0.25
    RT_Cylinder cylinder(true, &blue_mat);
    t.setTranslation({-2.5, 0.25, -2.0}); t.setScale({0.3, 0.75, 0.3}); cylinder.setTransform(t); t.reset();

    // Cone — half_angle=0.5rad, k=tan(0.5)≈0.546, height=1.0
    RT_Cone cone(true, &red_mat);
    t.setTranslation({2.0, 0.5, -2.0}); t.setScale({0.546, 1.0, 0.546}); cone.setTransform(t); t.reset();

    RT_PointLight light1(Point3d(-2.0, 2.0, 3.0), Color(1.0, 1.0, 1.0), 1.0);

    // Scene
    scene.addObject(&ground);
    scene.addObject(&back_wall);
    scene.addObject(&mirror_wall);
    scene.addObject(&sphere_center);
    scene.addObject(&sphere_left);
    scene.addObject(&sphere_right);
    scene.addObject(&cylinder);
    scene.addObject(&cone);

    scene.addLight(&light1);

    RT_Renderer renderer(800, 16.0/9.0, 50, 10);
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.setBackground(Color(0.0, 0.0, 0.0));
    renderer.render(true);
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "mirror.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "mirror.ppm");
    assert(file.good());
    std::cout << "mirror.ppm generated successfully\n";
    return 0; */
}
