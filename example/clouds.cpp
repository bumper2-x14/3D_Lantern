#include <fstream>
#include <cassert>
#include <iostream>
#include "ray_tracer/RT_Camera.h"
#include "ray_tracer/RT_Renderer.h"
#include "ray_tracer/RT_Lambertian.h"
#include "ray_tracer/RT_Sphere.h"
#include "ray_tracer/RT_Cylinder.h"
#include "ray_tracer/RT_Cone.h"
#include "ray_tracer/RT_Box.h"
#include "ray_tracer/RT_Medium.h"
#include "assets/perlin_texture.h"
#include "assets/color_texture.h"
#include "assets/image_texture.h"
#include "ray_tracer/RT_DirectionalLight.h"

int main() {
   /* RT_Scene scene;

    // Camera — low angle looking up slightly to emphasize the sky and clouds
    RT_Camera cam(
        Point3d(0.0, 0.8, 3.5),   // eye — slightly elevated, pulled back
        Point3d(0.0, 0.6, 0.0),   // look at — center of scene
        Vec3d(0, 1, 0),
        45.0, 0.0, 10.0
    );

    // -------------------------------------------------------------------------
    // Materials
    // -------------------------------------------------------------------------
    RT_Lambertian mat_ground    (new PerlinTexture(TERRAIN,  6));   // grass/dirt/snow
    RT_Lambertian mat_tree_bark (new PerlinTexture(WOOD,    18));   // tree trunks
    RT_Lambertian mat_leaves    (new ColorTexture(Color(0.0, 0.9, 0.0)));   // foliage (green tinted below)
    RT_Lambertian mat_rock      (new PerlinTexture(WARPED,  12));   // boulder
    RT_Lambertian mat_snow      (new PerlinTexture(ICE,      6));   // snow cap on mountain cone
    RT_Lambertian mat_mountain  (new PerlinTexture(TERRAIN, 10));   // mountain body

    // Invisible boundary material (never rendered directly)
    RT_Lambertian mat_dummy(new PerlinTexture(NOISE, 1));

    // -------------------------------------------------------------------------
    // Ground — huge sphere trick (flat-looking terrain)
    // -------------------------------------------------------------------------
    RT_Sphere ground(&mat_ground);
    TRSTransformd t;
    t.setTranslation({0.0, -100.5, 0.0});
    t.setScale({100.0, 100.0, 100.0});
    ground.setTransform(t); t.reset();

    // -------------------------------------------------------------------------
    // Mountain (cone) — centre-back
    // -------------------------------------------------------------------------
    RT_Cone mountain(true, &mat_mountain);
    t.setTranslation({0.0, 0.5, -4.0});
    t.setScale({1.2, 2.2, 1.2});
    mountain.setTransform(t); t.reset();

    // Snow cap — smaller cone on top of mountain
    RT_Cone snow_cap(true, &mat_snow);
    t.setTranslation({0.0, 1.6, -4.0});
    t.setScale({0.45, 0.8, 0.45});
    snow_cap.setTransform(t); t.reset();

    // -------------------------------------------------------------------------
    // Trees — cylinder trunk + sphere canopy (left and right)
    // -------------------------------------------------------------------------
    RT_Cylinder trunk_l(true, &mat_tree_bark);
    t.setTranslation({-2.2, 0.0, -1.5}); t.setScale({0.1, 0.55, 0.1});
    trunk_l.setTransform(t); t.reset();

    RT_Sphere canopy_l(&mat_leaves);
    t.setTranslation({-2.2, 0.75, -1.5}); t.setScale({0.38, 0.38, 0.38});
    canopy_l.setTransform(t); t.reset();

    RT_Cylinder trunk_r(true, &mat_tree_bark);
    t.setTranslation({2.4, 0.0, -1.8}); t.setScale({0.1, 0.5, 0.1});
    trunk_r.setTransform(t); t.reset();

    RT_Sphere canopy_r(&mat_leaves);
    t.setTranslation({2.4, 0.7, -1.8}); t.setScale({0.35, 0.35, 0.35});
    canopy_r.setTransform(t); t.reset();

    // A smaller background tree
    RT_Cylinder trunk_back(true, &mat_tree_bark);
    t.setTranslation({-1.2, 0.0, -3.0}); t.setScale({0.08, 0.45, 0.08});
    trunk_back.setTransform(t); t.reset();

    RT_Sphere canopy_back(&mat_leaves);
    t.setTranslation({-1.2, 0.62, -3.0}); t.setScale({0.28, 0.28, 0.28});
    canopy_back.setTransform(t); t.reset();

    // -------------------------------------------------------------------------
    // Boulder (sphere) — foreground right
    // -------------------------------------------------------------------------
    RT_Sphere rock(&mat_rock);
    t.setTranslation({1.3, -0.22, 0.8}); t.setScale({0.28, 0.28, 0.28});
    rock.setTransform(t); t.reset();

    // -------------------------------------------------------------------------
    // CLOUDS  (RT_Medium with CLOUD perlin texture)
    //
    // Each cloud is a slightly squashed sphere boundary wrapped in a medium.
    // Boundaries are NOT added to the scene — only the mediums are.
    // -------------------------------------------------------------------------

    ImageTexture* cld = new ImageTexture(IMG_DIR "cloud_tex.png");

    // Cloud 1 — large, centre-left, mid sky
    RT_Sphere cloud_b1(&mat_dummy);
    t.setTranslation({-1.2, 2.8, -3.5}); t.setScale({1.1, 0.5, 0.8});
    cloud_b1.setTransform(t); t.reset();
    RT_Medium cloud1(&cloud_b1, 2.35, new ImageTexture(IMG_DIR "cloud_tex.png"));

    // Cloud 2 — medium, right side, slightly higher
    RT_Sphere cloud_b2(&mat_dummy);
    t.setTranslation({1.8, 3.3, -4.5}); t.setScale({0.9, 0.42, 0.7});
    cloud_b2.setTransform(t); t.reset();
    RT_Medium cloud2(&cloud_b2, 2.3, new ImageTexture(IMG_DIR "cloud_tex.png"));

    // Cloud 3 — small, far background
    RT_Sphere cloud_b3(&mat_dummy);
    t.setTranslation({0.4, 3.8, -6.0}); t.setScale({0.6, 0.28, 0.5});
    cloud_b3.setTransform(t); t.reset();
    RT_Medium cloud3(&cloud_b3, 2.25, new ImageTexture(IMG_DIR "cloud_tex.png"));

    // Cloud 4 — wispy, upper left
    RT_Sphere cloud_b4(&mat_dummy);
    t.setTranslation({-2.8, 4.2, -5.0}); t.setScale({0.75, 0.3, 0.6});
    cloud_b4.setTransform(t); t.reset();
    RT_Medium cloud4(&cloud_b4, 2.2, new ImageTexture(IMG_DIR "cloud_tex.png"));

    // Cloud 5 — close overhead, slightly right (gives depth)
    RT_Sphere cloud_b5(&mat_dummy);
    t.setTranslation({0.8, 2.3, -1.8}); t.setScale({0.7, 0.32, 0.55});
    cloud_b5.setTransform(t); t.reset();
    RT_Medium cloud5(&cloud_b5, 0.4, new ImageTexture(IMG_DIR "cloud_tex.png"));

    RT_DirectionalLight light(
        Vec3d(0.0, 1.0, 1.0),
        Color(1.0, 0.95, 0.85),
        1.5
    );
    
    // -------------------------------------------------------------------------
    // Scene assembly
    // -------------------------------------------------------------------------
    // Terrain & solid objects
    scene.addObject(&ground);
    scene.addObject(&mountain);
    scene.addObject(&snow_cap);
    scene.addObject(&trunk_l);
    scene.addObject(&canopy_l);
    scene.addObject(&trunk_r);
    scene.addObject(&canopy_r);
    scene.addObject(&trunk_back);
    scene.addObject(&canopy_back);
    scene.addObject(&rock);

    // Clouds (boundaries NOT added — only mediums)
    scene.addObject(&cloud1);
    scene.addObject(&cloud2);
    scene.addObject(&cloud3);
    scene.addObject(&cloud4);
    scene.addObject(&cloud5);

    scene.addLight(&light);

    // -------------------------------------------------------------------------
    // Renderer — high sample count to resolve volumetric noise cleanly
    // -------------------------------------------------------------------------
    RT_Renderer renderer(800, 16.0 / 9.0, 200, 30);
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.setBackground(Color(0.0, 0.0, 0.0));
    renderer.render();
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "clouds.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "clouds.ppm");
    assert(file.good());
    std::cout << "clouds.ppm was generated successfully\n";
    return 0;
    */
}