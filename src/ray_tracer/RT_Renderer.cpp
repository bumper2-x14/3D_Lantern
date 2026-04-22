#include <iostream>
#include <cassert>
#include <iomanip>

#include "RT_Renderer.h"
#include "math/utility.h"
#include "RT_BVH.h"

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

void RT_Renderer::setScene(RT_Scene* _scene) { scene = _scene; }

void RT_Renderer::setBackground(const Color& _background) { background = _background; }

Color RT_Renderer::traceRay(const Rayd& ray, int recursive_depth, RT_Object* accel) const {
    if (recursive_depth <= 0)
        return Color(0, 0, 0);

    RT_Record rec;
    if (!accel->rayIntersect(ray, Intervald(0.001, infinity<double>), rec))
        return background;

    Color attenuation;
    Rayd  scattered;
    if (!rec.material || !rec.material->rayScatter(ray, rec, attenuation, scattered))
        return Color(0, 0, 0);  // hit something that doesn't scatter — return black

    // direct lighting from point lights
    Color direct(0, 0, 0);
    for (const auto& light : scene->getLights()) {
        switch (light->type) {
            case POINTLIGHT: {
                Vec3d to_light = light->position - rec.p;
                double dist = to_light.length();
                Vec3d light_dir = normalize(to_light);

                RT_Record shadow_rec;
                bool in_shadow = accel->rayIntersect(Rayd(rec.p, light_dir), Intervald(0.001, dist - 0.001),shadow_rec);
                if (!in_shadow) {
                    double cos_theta = std::max(0.0, dot(rec.normal, light_dir));
                    direct += attenuation * light->radiate(dist) * cos_theta;
                }
                break;
            }
            
            case DIRECTIONALLIGHT: {
                // Shadow ray shoots infinitely far in the light direction
                // so dist is infinity — any hit blocks the light
                RT_Record shadow_rec;
                bool in_shadow = accel->rayIntersect(
                    Rayd(rec.p, light->direction),
                    Intervald(0.001, infinity<double>), shadow_rec);

                if (!in_shadow) {
                    double cos_theta = std::max(0.0, dot(rec.normal, light->direction));
                    direct += attenuation * light->radiate() * cos_theta;
                }
            }
        }
    }
    // indirect bounce
    Color indirect = attenuation * traceRay(scattered, recursive_depth - 1, accel);

    return direct + indirect;
}

void RT_Renderer::singleThreadRender() {
    if (!camera) {
        std::cout<<"RT_renderer::render -> camera was not set\n"<<std::endl;
        return;
    }

    camera->initialize(aspect_ratio, img_width, img_height, sample_per_pixel); // Init camera
    RT_Object* accelerated = new BVHNode(scene->getObjects(), 0, scene->getObjects().size());

    int total = img_height * img_width;
    int done = 0;

    for (int j = 0; j < img_height; j++){
        for (int i = 0; i < img_width; i++){
            Color pix_color(0.0, 0.0 ,0.0);
            for (int inner_j = 0; inner_j < sqrt_spp; inner_j++){
                for (int inner_i = 0; inner_i < sqrt_spp; inner_i++){
                    Rayd ray = camera->generateRay(i, j, inner_i, inner_j);
                    pix_color += traceRay(ray, depth, accelerated);
                }
            }
            img_buffer[j * img_width + i] = sample_scale * pix_color;
            done++;
        }
        std::cout << "\rRendering: " 
                  << std::fixed << std::setprecision(1)
                  << (100.0 * done / total) << "% "
                  << "(" << done << "/" << total << " px)"
                  << std::flush;
    }
        
    std::cout << "\nDone.\n";
}

// in case a memory leak is detected it sould be from accelerated
void RT_Renderer::multiThreadRender() {
    if (!camera) {
        std::cout<<"RT_renderer::render -> camera was not set\n"<<std::endl;
        return;
    }

    camera->initialize(aspect_ratio, img_width, img_height, sample_per_pixel); // Init camera
    RT_Object* accelerated = new BVHNode(scene->getObjects(), 0, scene->getObjects().size());

    int thread_count = std::thread::hardware_concurrency();

    if (thread_count == 0) thread_count = 1;

    std::vector<std::thread> threads;
    int row_per_thread = img_height / thread_count;
    std::clog<<"Rendering with "<< thread_count <<" threads...\n";

    int total = img_height * img_width;
    std::atomic<int> done {0};
    std::atomic<bool>finished{false};

    std::thread prog_monitor([&]() {
        while (!finished.load(std::memory_order_relaxed)) {
            int d = done.load(std::memory_order_relaxed);
            std::cout << "\rRendering: " << std::fixed << std::setprecision(1)
                      << (100.0 * d / total) << "%" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "\rRendering: 100.0%\nDone.\n";
    });
    
    for (int t = 0; t < thread_count; t++){
        int start_y = t * row_per_thread;
        int end_y = (t == thread_count-1) ? img_height : start_y + row_per_thread;

        threads.emplace_back(&RT_Renderer::renderWorker, this, start_y, end_y, accelerated, std::ref(done));
    }
    
    for (auto& th : threads) {
        th.join();
    }

    finished.store(true);
    prog_monitor.join();

    delete accelerated;
}

void RT_Renderer::renderWorker(int start_y, int end_y, RT_Object* accel, std::atomic<int>& done) {
    for (int j = start_y; j < end_y; j++){
        for (int i = 0; i < img_width; i++){
            Color pix_color (0.0, 0.0, 0.0);
            for (int inner_j = 0; inner_j < sqrt_spp; inner_j++){
                for (int inner_i = 0; inner_i < sqrt_spp; inner_i++){
                    Rayd ray = camera->generateRay(i, j, inner_i, inner_j);
                    pix_color += traceRay(ray, depth, accel);
                }
            }
            img_buffer[j * img_width + i] = sample_scale * pix_color;
            done++;
        }
    }
}

void RT_Renderer::render(bool threaded) {
    if (!threaded){
        singleThreadRender();
    }
    else {
        multiThreadRender();
    }
}

void RT_Renderer::writePPM(const std::string& path) const {
    std::ofstream file(path);
    file << "P3\n" << img_width << " " << img_height << "\n255\n";
    for (int i = 0; i < img_width * img_height; i++) {
        writeColor(file, img_buffer[i]);
    }
}

/*
void RT_Renderer::regressionTest() {

    // --- Stubs ---------------------------------------------------
    struct NullMaterial : public RT_Material {
        bool rayScatter(const Rayd&, const RT_Record&, Color&, Rayd&) const override {
            return false;
        }
    };

    struct AlwaysHitObject : public RT_Object {
        NullMaterial *mat;
        bool rayIntersect(const Rayd& ray, const Intervald& t, RT_Record& rec) const override {
            rec.t = (t.min + t.max) / 2.0;
            rec.p = ray.at(rec.t);
            rec.material = mat;
            return true;
        }
    };
    // -------------------------------------------------------------

    // Test constructor initializes correctly
    RT_Renderer renderer(800, 16.0/9.0, 100, 10);
    assert(renderer.img_width == 800);
    assert(renderer.img_height > 0);
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
    renderer2.render();

    // Test full render pipeline
    RT_Camera cam2(
        Point3d(0, 0,  1),
        Point3d(0, 0, -1),
        Vec3d(0, 1, 0),
        50.0, 0.0, 10.0
    );

    AlwaysHitObject obj;
    RT_ObjectList scene2;
    scene2.add(&obj);

    RT_Renderer renderer3(800, 16.0/9.0, 100, 20);
    renderer3.setCamera(&cam2);
    renderer3.setScene(&scene2);
    renderer3.setBackground(Color(0.5, 0.7, 1.0));
    renderer3.render();

    // Test writePPM produces a valid file
    renderer3.writePPM("test_output.ppm");
    std::ifstream file("test_output.ppm");
    assert(file.good());
    std::string header;
    std::getline(file, header);
    assert(header == "P3");

    std::cout << "All RT_Renderer tests passed successfully\n";
}
*/