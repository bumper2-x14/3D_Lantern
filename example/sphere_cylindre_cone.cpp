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


int main() {
    RT_Camera cam(
        Point3d(0, 0,  1),
        Point3d(0, 0, -1),
        Vec3d(0, 1, 0),
        50.0, 0.0, 10.0
    );

    // Materials
    RT_Metallic mat_center(Color(0.4, 0.4, 0.8), 0);
    RT_Lambertian mat_left  (Color(0.8, 0.3, 0.3));
    RT_Dielectric mat_right (1.490);
    RT_Metallic mat_small (Color(0.8, 0.8, 0.3), 0.6);
    RT_Lambertian mat_ground(new CheckerTexture(Color(1.0, 1.0, 1.0), Color(0.0, 0.0, 0.0), 10));
    RT_Metallic mat_cyl_left (Color(0.2, 0.8, 0.4), 0.1);  
    RT_Lambertian mat_cyl_right(Color(0.9, 0.5, 0.2));       
    RT_Dielectric mat_cyl_back (1.330); 
    RT_Lambertian mat_cone_left (Color(0.6, 0.2, 0.7));   
    RT_Metallic   mat_cone_right(Color(0.9, 0.7, 0.2), 0.7);  
    RT_Lambertian mat_cone_back (Color(0.3, 0.6, 0.8));

    // Spheres
    RT_Sphere sphere_center(Point3d( 0.0,  -0.3,  -1.0),   0.3,   &mat_center);
    RT_Sphere sphere_left  (Point3d(-0.8,  -0.3,  -1.2),   0.3,   &mat_left);
    RT_Sphere sphere_right (Point3d( 0.8,  -0.3,  -1.2),   0.3,   &mat_right);
    RT_Sphere sphere_small (Point3d( 0.0,  0.8,  -1.5),   0.25,  &mat_small);
    RT_Sphere sphere_ground(Point3d( 0.0, -100.5, -1.0), 100.0,  &mat_ground);
    
    // Cylinders
    RT_Cylinder cyl_left (Point3d(-1.5, -0.5, -1.5), 0.18, -0.5, 1.4, true, &mat_cyl_left);
    RT_Cylinder cyl_right(Point3d( 1.5, -0.5, -1.5), 0.18, -0.5, 1.4, true, &mat_cyl_right);

    // Cone
    RT_Cone cone_left(Point3d(-1.5, 0.5, -1.0), 0.348, -1.0, 0.0, true, &mat_cone_left);
    RT_Cone cone_right(Point3d( 1.5, 0.5, -1.0), 0.348, -1.0, 0.0, true, &mat_cone_right);

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
