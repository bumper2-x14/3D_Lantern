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
#include "ray_tracer/RT_Box.h"
#include "ray_tracer/RT_Quad.h"
#include "ray_tracer/RT_PointLight.h"

int main() {
    RT_Scene scene;

    // Camera — inside the chamber looking inward
    RT_Camera cam(
        Point3d(0.0, 0.5, 1.5),   // eye: slightly above center, near front wall
        Point3d(0.0, 0.3, -1.0),  // lookat: toward the objects
        Vec3d  (0.0, 1.0,  0.0),
        60.0, 0.0, 1.0
    );

    // ── materials ────────────────────────────────────────────────────────────
    RT_Metallic  mirror_mat  (Color(1.00, 1.00, 1.00), 0.00); // perfect mirror
    RT_Metallic  tinted_left (Color(0.85, 0.95, 1.00), 0.0); // cool blue tint
    RT_Metallic  tinted_right(Color(1.00, 0.90, 0.85), 0.0); // warm red tint
    RT_Lambertian floor_mat  (Color(0.60, 0.60, 0.60)); // matte grey floor
    RT_Lambertian ceil_mat   (Color(0.80, 0.80, 0.80)); // lighter ceiling

    // Objects inside
    RT_Metallic   metal_mat  (Color(0.90, 0.80, 0.60), 0.05); // gold metallic
    RT_Dielectric glass_mat  (1.50);
    RT_Lambertian red_mat    (Color(0.80, 0.15, 0.15));
    RT_Lambertian blue_mat   (Color(0.15, 0.25, 0.80));

    // ── chamber walls — enclosed box, 6 quads ────────────────────────────────
    // Room: x[-3,3]  y[-1,3]  z[-4,2]
    // All inward-facing so normals point into the room

    TRSTransformd t;

    // Floor — matte so light pools visibly
    RT_Quad floor_q(&floor_mat);
    t.setTranslation({0.0, -1.0,  0.0});
    t.setRotation   ({-90.0, 0.0, 0.0});
    t.setScale      ({6.0, 6.0, 1.0});
    floor_q.setTransform(t); t.reset();

    // Ceiling
    RT_Quad ceil_q(&ceil_mat);
    t.setTranslation({0.0,  3.0,  0.0});
    t.setRotation   ({90.0, 0.0, 0.0});
    t.setScale      ({6.0, 6.0, 1.0});
    ceil_q.setTransform(t); t.reset();

    // Back wall — mirror
    RT_Quad back_q(&mirror_mat);
    t.setTranslation({0.0,  1.0, -4.0});
    t.setRotation   ({0.0,  0.0,  0.0});
    t.setScale      ({6.0, 4.0,  1.0});
    back_q.setTransform(t); t.reset();

    // Front wall — mirror (behind camera, catches back reflections)
    RT_Quad front_q(&mirror_mat);
    t.setTranslation({0.0,  1.0,  2.0});
    t.setRotation   ({0.0, 180.0, 0.0});
    t.setScale      ({6.0, 4.0,  1.0});
    front_q.setTransform(t); t.reset();

    // Left wall — cool tinted mirror
    RT_Quad left_q(&tinted_left);
    t.setTranslation({-3.0, 1.0, -1.0});
    t.setRotation   ({0.0,  90.0, 0.0});
    t.setScale      ({6.0, 4.0,  1.0});
    left_q.setTransform(t); t.reset();

    // Right wall — warm tinted mirror
    RT_Quad right_q(&tinted_right);
    t.setTranslation({ 3.0, 1.0, -1.0});
    t.setRotation   ({0.0, -90.0, 0.0});
    t.setScale      ({6.0, 4.0,  1.0});
    right_q.setTransform(t); t.reset();

    // ── objects inside the chamber ────────────────────────────────────────────

    // Gold metallic sphere — centrepiece
    RT_Sphere sphere_center(&metal_mat);
    t.setTranslation({ 0.0, -0.4, -1.5});
    t.setScale      ({ 0.6,  0.6,  0.6});
    sphere_center.setTransform(t); t.reset();

    // Glass sphere — left
    RT_Sphere sphere_glass(&glass_mat);
    t.setTranslation({-1.2, -0.5, -2.0});
    t.setScale      ({ 0.5,  0.5,  0.5});
    sphere_glass.setTransform(t); t.reset();

    // Red matte cylinder — right pillar
    RT_Cylinder cylinder(true, &red_mat);
    t.setTranslation({ 1.5, 0.0, -2.5});
    t.setScale      ({ 0.25, 1.0, 0.25});
    cylinder.setTransform(t); t.reset();

    // ── lights ────────────────────────────────────────────────────────────────

    // Main overhead light — near ceiling centre
    RT_PointLight light_main(Point3d(0.0, 2.5, -1.0),
                             Color(1.0, 1.0, 1.0), 0.05);


    // ── scene ─────────────────────────────────────────────────────────────────
    // Walls
    scene.addObject(&floor_q);
    scene.addObject(&ceil_q);
    scene.addObject(&back_q);
    scene.addObject(&front_q);
    scene.addObject(&left_q);
    scene.addObject(&right_q);

    // Objects
    scene.addObject(&sphere_center);
    scene.addObject(&sphere_glass);
    scene.addObject(&cylinder);

    scene.addLight(&light_main);

    RT_Renderer renderer(800, 16.0/9.0, 400, 50); // high depth for mirror bounces
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.setBackground(Color(0.0, 0.0, 0.0)); // black bg — fully enclosed

    renderer.render(true);
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "mirror_chamber.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "mirror_chamber.ppm");
    assert(file.good());
    std::cout << "mirror_chamber.ppm generated successfully\n";
    return 0;
}