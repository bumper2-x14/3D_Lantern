#include <fstream>
#include <cassert>
#include <iostream>
#include <string>
#include "ray_tracer/RT_Camera.h"
#include "ray_tracer/RT_Renderer.h"
#include "ray_tracer/RT_Lambertian.h"
#include "ray_tracer/RT_Metallic.h"
#include "ray_tracer/RT_Sphere.h"
#include "ray_tracer/RT_Mesh.h"
#include "ray_tracer/RT_PointLight.h"
#include "ray_tracer/RT_ObjectList.h"
#include "assets/model.h"

int main() {
    RT_Camera cam(
        Point3d(0, 2, 8),
        Point3d(0, 0, 0),
        Vec3d  (0, 1, 0),
        60.0, 0.0, 10.0
    );

    // --- Materials ---
    RT_Lambertian mat_ground(Color(0.4, 0.4, 0.4));
    RT_Metallic mat_teapot(Color(0.8, 0.6, 0.2), 0.1);

    // --- Ground ---
    RT_Sphere ground(&mat_ground);
    TRSTransformd t;
    t.setTranslation({0.0, -100.5, 0.0});
    t.setScale({100.0, 100.0, 100.0});
    ground.setTransform(t); t.reset();

    // --- Teapot ---
    Model teapotModel(std::string(RESOURCE_DIR) + "teapot.obj");
    RT_Mesh teapot(teapotModel.getMesh(), &mat_teapot);
    t.setTranslation({-0.34, 2.05, 0.053});
    t.setScale({0.05, 0.05, 0.05});
    teapot.setTransform(t); t.reset();

    // --- Light ---
    /*
    RT_PointLight light(
        Point3d(-7.0, 5.0, 0.0),
        Color(1.0, 1.0, 1.0),
        1.5
    );
    */

    // --- Scene ---
    RT_ObjectList scene;
    scene.add(&ground);
    scene.add(&teapot);

    // --- Renderer ---
    RT_Renderer renderer(800, 16.0/9.0, 100, 20);
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.setBackground(Color(0.5, 0.7, 1.0));
    //renderer.p_lights.push_back(&light);
    renderer.render();
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "teapot.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "teapot.ppm");
    assert(file.good());
    std::cout << "teapot.ppm was generated successfully\n";
}