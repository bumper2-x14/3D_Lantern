#include <iostream>
#include <cassert>

#include "RT_Renderer.h"
#include "math/utility.h"

RT_Renderer::RT_Renderer(int _width, double _aspect_ratio, int _samples_per_pixel, int _depth):
    camera(nullptr), scene(nullptr), background(Color(0,0,0)),img_width(_width), 
    aspect_ratio(_aspect_ratio), sample_per_pixel(_samples_per_pixel), depth(_depth) {
        
        // Height is calculated from width and aspect ratio of image
        img_height = int(img_width / aspect_ratio);
        img_height = (img_height < 1) ? 1 : img_height; 

        sqrt_spp = int(std::sqrt(sample_per_pixel));
        sample_scale = 1.0 / (sqrt_spp * sqrt_spp);
    
        // Allocate image buffer by the given width and height
        img_buffer = new Color[img_width * img_height];
}

RT_Renderer::~RT_Renderer() {
    delete [] img_buffer;
}

void RT_Renderer::setCamera(RT_Camera* _camera) { camera = _camera; }

void RT_Renderer::setScene(RT_ObjectList* _scene) { scene = _scene; }

void RT_Renderer::setBackground(const Color& _background) { background = _background; }

Color RT_Renderer::traceRay(const Rayd& ray, int recursive_depth) const {
    if (recursive_depth <= 0)
        return Color(0, 0, 0);

    RT_Record rec;
    if (scene->rayIntersect(ray, Intervald(0.001, infinity<double>), rec)) {
        // TODO: return material color once materials are added
        // for now visualize normals without recursive tracing
        return 0.5 * Color(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
    }
    return background;
}

void RT_Renderer::render() {
    if (!camera || !scene) {
        std::cout<<"RT_renderer::render -> camera or scene was not set\n"<<std::endl;
        return;
    }

    camera->initialize(aspect_ratio, img_width, img_height, sample_per_pixel);

    for (int j = 0; j < img_height; j++){
        for (int i = 0; i < img_width; i++){
            Color pix_color(0.0, 0.0 ,0.0);
            for (int inner_j = 0; inner_j < sqrt_spp; inner_j++){
                for (int inner_i = 0; inner_i < sqrt_spp; inner_i++){
                    Rayd ray = camera->generateRay(i, j, inner_i, inner_j);
                    pix_color += traceRay(ray, depth);
                }
            }
            img_buffer[j * img_width + i] = sample_scale * pix_color;
        }
    }
}

void RT_Renderer::writePPM(const std::string& path) const {
    std::ofstream file(path);
    file << "P3\n" << img_width << " " << img_height << "\n255\n";
    for (int i = 0; i < img_width * img_height; i++) {
        writeColor(file, img_buffer[i]);
    }
}

#include "ray_tracer/RT_Sphere.h"

void RT_Renderer::regressionTest() {
    // Test constructor initializes correctly
    RT_Renderer renderer(800, 16.0/9.0, 4, 50);
    assert(renderer.img_width == 800);
    assert(renderer.img_height > 0);
    assert(renderer.sqrt_spp == 2);
    assert(std::abs(renderer.sample_scale - 0.25) < 1e-6);
    assert(renderer.img_buffer != nullptr);
    assert(renderer.camera == nullptr);
    assert(renderer.scene == nullptr);

    // Test setters
    RT_Camera cam;
    renderer.setCamera(&cam);
    assert(renderer.camera == &cam);

    RT_ObjectList scene;
    renderer.setScene(&scene);
    assert(renderer.scene == &scene);

    Color bg(0.5, 0.7, 1.0);
    renderer.setBackground(bg);
    assert(renderer.background.r == 0.5f);
    assert(renderer.background.g == 0.7f);
    assert(renderer.background.b == 1.0f);

    // Test render without camera/scene does not crash
    RT_Renderer renderer2(100, 16.0/9.0, 4, 10);
    renderer2.render(); // should just print warning and return

    // Test full render pipeline with a simple scene
    RT_Camera cam2(
        Point3d(0, 0, 0),
        Point3d(0, 0, -1),
        Vec3d(0, 1, 0),
        90.0, 0.0, 10.0
    );
    RT_ObjectList scene2;
    RT_Sphere sphere(Point3d(0, 0, -1), 0.5);
    scene2.add(&sphere);

    RT_Renderer renderer3(800, 16.0/9.0, 4, 10);
    renderer3.setCamera(&cam2);
    renderer3.setScene(&scene2);
    renderer3.setBackground(Color(0.5, 0.7, 1.0));
    renderer3.render();

    // Check buffer was written — center pixel should not be black (hits sphere)
    int cx = 400, cy = renderer3.img_height / 2;
    Color center = renderer3.img_buffer[cy * renderer3.img_width + cx];
    assert(center.r > 0 || center.g > 0 || center.b > 0);

    // Test writePPM produces a file
    renderer3.writePPM("test_output.ppm");
    std::ifstream file("test_output.ppm");
    assert(file.good());
    std::string header;
    std::getline(file, header);
    assert(header == "P3");

    std::cout << "All RT_Renderer tests passed successfully\n";
}