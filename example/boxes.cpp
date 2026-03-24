#include <fstream>
#include <cassert>  
#include <iostream>  
#include <string>
#include "ray_tracer/RT_Camera.h"
#include "ray_tracer/RT_Renderer.h"
#include "ray_tracer/RT_Lambertian.h"
#include "ray_tracer/RT_Dielectric.h"
#include "ray_tracer/RT_Metallic.h"
#include "ray_tracer/RT_Box.h"

int main() {
    // Materials
    RT_Metallic   mat_silver (Color(0.9, 0.9, 0.95), 0.0);   // perfect silver
    RT_Metallic   mat_gold   (Color(1.0, 0.85, 0.3), 0.05);  // gold
    RT_Metallic   mat_copper (Color(0.95, 0.64, 0.54), 0.1); // copper
    RT_Metallic   mat_brushed(Color(0.7, 0.7, 0.8),   0.3);  // brushed metal
    RT_Lambertian mat_red    (Color(0.8, 0.1, 0.1));          // red matte
    RT_Lambertian mat_blue   (Color(0.1, 0.2, 0.8));          // blue matte
    RT_Lambertian mat_green  (Color(0.2, 0.7, 0.2));          // green matte
    RT_Lambertian mat_wood   (Color(0.6, 0.4, 0.2));          // wood
    RT_Lambertian mat_ground (Color(0.75, 0.70, 0.65));       // ground
    RT_Dielectric mat_glass  (1.520);                          // glass
    RT_Dielectric mat_diamond(2.417);                          // diamond

    // Ground plane — one big flat box
    RT_Box ground(
        Point3d(-8.0, -0.52, -8.0),
        Point3d( 8.0, -0.50,  2.0),
        &mat_ground
    );

    // ── back row ──────────────────────────────────────────────────────────────

    // Tall gold pillar
    RT_Box box_gold(
        Point3d(-2.2, -0.5, -2.8),
        Point3d(-1.6,  1.2, -2.2),
        &mat_gold
    );

    // Wide flat red slab
    RT_Box box_red(
        Point3d(-0.9, -0.5, -2.6),
        Point3d( 0.9, -0.1, -2.0),
        &mat_red
    );

    // Tall glass tower
    RT_Box box_glass(
        Point3d( 1.4, -0.5, -2.8),
        Point3d( 2.0,  1.4, -2.2),
        &mat_glass
    );

    // ── middle row ────────────────────────────────────────────────────────────

    // Silver cube
    RT_Box box_silver(
        Point3d(-2.0, -0.5, -1.6),
        Point3d(-1.4,  0.1, -1.0),
        &mat_silver
    );

    // Stacked blue box (sitting on a wood box below)
    RT_Box box_wood_base(
        Point3d(-0.4, -0.5, -1.8),
        Point3d( 0.4, -0.2, -1.1),
        &mat_wood
    );
    RT_Box box_blue_top(
        Point3d(-0.25, -0.2, -1.7),
        Point3d( 0.25,  0.2, -1.2),
        &mat_blue
    );

    // Copper box on the right
    RT_Box box_copper(
        Point3d( 1.3, -0.5, -1.6),
        Point3d( 2.0,  0.3, -1.0),
        &mat_copper
    );

    // ── front row ─────────────────────────────────────────────────────────────

    // Small diamond cube — centrepiece
    RT_Box box_diamond(
        Point3d(-0.18, -0.5, -0.9),
        Point3d( 0.18, -0.1, -0.55),
        &mat_diamond
    );

    // Brushed metal low box left
    RT_Box box_brushed(
        Point3d(-1.8, -0.5, -0.8),
        Point3d(-1.0, -0.2, -0.3),
        &mat_brushed
    );

    // Green tall thin box right
    RT_Box box_green(
        Point3d( 1.0, -0.5, -0.9),
        Point3d( 1.4,  0.7, -0.5),
        &mat_green
    );

    // Small red accent cube front-left
    RT_Box box_red2(
        Point3d(-0.7, -0.5, -0.5),
        Point3d(-0.4, -0.2, -0.2),
        &mat_red
    );

    // Small gold accent cube front-right
    RT_Box box_gold2(
        Point3d( 0.4, -0.5, -0.5),
        Point3d( 0.7, -0.2, -0.2),
        &mat_gold
    );

    RT_ObjectList scene;
    scene.add(&ground);
    scene.add(&box_gold);
    scene.add(&box_red);
    scene.add(&box_glass);
    scene.add(&box_silver);
    scene.add(&box_wood_base);
    scene.add(&box_blue_top);
    scene.add(&box_copper);
    scene.add(&box_diamond);
    scene.add(&box_brushed);
    scene.add(&box_green);
    scene.add(&box_red2);
    scene.add(&box_gold2);

    RT_Camera cam(
        Point3d(0, 1.8, 2.5),   // elevated, pulled back
        Point3d(0, 0.1, -1.5),  // look toward middle of scene
        Vec3d  (0, 1,   0),     // up
        45.0, 0.0, 10.0
    );

    RT_Renderer renderer(800, 16.0/9.0, 200, 50);
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.setBackground(Color(0.5, 0.7, 1.0));
    renderer.render();
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "boxes.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "boxes.ppm");
    assert(file.good());
    std::cout << "boxes.ppm generated successfully\n";
    return 0;
}