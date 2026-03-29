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
    RT_Metallic   mat_silver (Color(0.9, 0.9, 0.95), 0.0);
    RT_Metallic   mat_gold   (Color(1.0, 0.85, 0.3), 0.05);
    RT_Metallic   mat_copper (Color(0.95, 0.64, 0.54), 0.1);
    RT_Metallic   mat_brushed(Color(0.7, 0.7, 0.8),   0.3);
    RT_Lambertian mat_red    (Color(0.8, 0.1, 0.1));
    RT_Lambertian mat_blue   (Color(0.1, 0.2, 0.8));
    RT_Lambertian mat_green  (Color(0.2, 0.7, 0.2));
    RT_Lambertian mat_wood   (Color(0.6, 0.4, 0.2));
    RT_Lambertian mat_ground (Color(0.75, 0.70, 0.65));
    RT_Dielectric mat_glass  (1.520);
    RT_Dielectric mat_diamond(2.417);

    RT_Box ground      (&mat_ground);
    RT_Box box_gold    (&mat_gold);
    RT_Box box_red     (&mat_red);
    RT_Box box_glass   (&mat_glass);
    RT_Box box_silver  (&mat_silver);
    RT_Box box_wood    (&mat_wood);
    RT_Box box_blue    (&mat_blue);
    RT_Box box_copper  (&mat_copper);
    RT_Box box_diamond (&mat_diamond);
    RT_Box box_brushed (&mat_brushed);
    RT_Box box_green   (&mat_green);
    RT_Box box_red2    (&mat_red);
    RT_Box box_gold2   (&mat_gold);

    TRSTransformd t;

    // ground: min(-8,-0.52,-8) max(8,-0.50,2)
    t.setTranslation({ 0.0,  -0.51, -3.0}); t.setScale({8.0,  0.01,  5.0 }); ground     .setTransform(t); t.reset();
    // box_gold: min(-2.2,-0.5,-2.8) max(-1.6,1.2,-2.2)
    t.setTranslation({-1.9,   0.35, -2.5}); t.setScale({0.3,  0.85,  0.3 }); box_gold   .setTransform(t); t.reset();
    // box_red: min(-0.9,-0.5,-2.6) max(0.9,-0.1,-2.0)
    t.setTranslation({ 0.0,  -0.3,  -2.3}); t.setScale({0.9,  0.2,   0.3 }); box_red    .setTransform(t); t.reset();
    // box_glass: min(1.4,-0.5,-2.8) max(2.0,1.4,-2.2)
    t.setTranslation({ 1.7,   0.45, -2.5}); t.setScale({0.3,  0.95,  0.3 }); box_glass  .setTransform(t); t.reset();
    // box_silver: min(-2.0,-0.5,-1.6) max(-1.4,0.1,-1.0)
    t.setTranslation({-1.7,  -0.2,  -1.3}); t.setScale({0.3,  0.3,   0.3 }); box_silver .setTransform(t); t.reset();
    // box_wood: min(-0.4,-0.5,-1.8) max(0.4,-0.2,-1.1)
    t.setTranslation({ 0.0,  -0.35, -1.45}); t.setScale({0.4, 0.15,  0.35}); box_wood   .setTransform(t); t.reset();
    // box_blue: min(-0.25,-0.2,-1.7) max(0.25,0.2,-1.2)
    t.setTranslation({ 0.0,   0.0,  -1.45}); t.setScale({0.25,0.2,   0.25}); box_blue   .setTransform(t); t.reset();
    // box_copper: min(1.3,-0.5,-1.6) max(2.0,0.3,-1.0)
    t.setTranslation({ 1.65, -0.1,  -1.3}); t.setScale({0.35, 0.4,   0.3 }); box_copper .setTransform(t); t.reset();
    // box_diamond: min(-0.18,-0.5,-0.9) max(0.18,-0.1,-0.55)
    t.setTranslation({ 0.0,  -0.3,  -0.725}); t.setScale({0.18,0.2,  0.175}); box_diamond.setTransform(t); t.reset();
    // box_brushed: min(-1.8,-0.5,-0.8) max(-1.0,-0.2,-0.3)
    t.setTranslation({-1.4,  -0.35, -0.55}); t.setScale({0.4, 0.15,  0.25}); box_brushed.setTransform(t); t.reset();
    // box_green: min(1.0,-0.5,-0.9) max(1.4,0.7,-0.5)
    t.setTranslation({ 1.2,   0.1,  -0.7}); t.setScale({0.2,  0.6,   0.2 }); box_green  .setTransform(t); t.reset();
    // box_red2: min(-0.7,-0.5,-0.5) max(-0.4,-0.2,-0.2)
    t.setTranslation({-0.55, -0.35, -0.35}); t.setScale({0.15,0.15,  0.15}); box_red2   .setTransform(t); t.reset();
    // box_gold2: min(0.4,-0.5,-0.5) max(0.7,-0.2,-0.2)
    t.setTranslation({ 0.55, -0.35, -0.35}); t.setScale({0.15,0.15,  0.15}); box_gold2  .setTransform(t); t.reset();

    RT_ObjectList scene;
    scene.add(&ground);
    scene.add(&box_gold);
    scene.add(&box_red);
    scene.add(&box_glass);
    scene.add(&box_silver);
    scene.add(&box_wood);
    scene.add(&box_blue);
    scene.add(&box_copper);
    scene.add(&box_diamond);
    scene.add(&box_brushed);
    scene.add(&box_green);
    scene.add(&box_red2);
    scene.add(&box_gold2);

    RT_Camera cam(
        Point3d(0, 1.8,  2.5),
        Point3d(0, 0.1, -1.5),
        Vec3d  (0, 1,    0),
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
