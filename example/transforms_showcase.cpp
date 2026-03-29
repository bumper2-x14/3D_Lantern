#include "ray_tracer/RT_Camera.h"
#include "ray_tracer/RT_Renderer.h"
#include "ray_tracer/RT_Lambertian.h"
#include "ray_tracer/RT_Dielectric.h"
#include "ray_tracer/RT_Metallic.h"
#include "ray_tracer/RT_Quad.h"
#include "ray_tracer/RT_Box.h"
#include "ray_tracer/RT_Sphere.h"
#include "ray_tracer/RT_Cylinder.h"
#include "ray_tracer/RT_Cone.h"
#include "assets/checker_texture.h"
#include "assets/image_texture.h"
#include "assets/perlin_texture.h"


int main() {
    RT_Camera cam(
        Point3d(0, 5, 5),
        Point3d(0, 1, -1),
        Vec3d(0, 2, 0),
        30.0, 0.0, 1.0
    );

    // Materials
    RT_Lambertian mat_ground  (Color(0.7, 0.7, 0.7));
    RT_Lambertian mat_red     (Color(0.8, 0.2, 0.2));
    RT_Lambertian mat_blue    (Color(0.2, 0.3, 0.8));
    RT_Lambertian mat_green   (Color(0.2, 0.7, 0.3));
    RT_Metallic   mat_silver  (Color(0.9, 0.9, 0.9), 0.05);
    RT_Metallic   mat_gold    (Color(1.0, 0.85, 0.3), 0.1);
    RT_Dielectric mat_glass   (1.5);
    RT_Lambertian mat_wall    (Color(0.4, 0.4, 0.6));

    // ── Ground ────────────────────────────────────────────────────────
    RT_Quad ground(&mat_ground);
    TRSTransformd t;
    t.setTranslation({0, 0, -2});
    t.setRotation   ({-90, 0, 0});
    t.setScale      ({10, 8, 1});
    ground.setTransform(t); t.reset();

    // ── Back wall ─────────────────────────────────────────────────────
    RT_Quad back_wall(&mat_wall);
    t.setTranslation({0, 3, -5});
    t.setScale      ({10, 6, 1});
    back_wall.setTransform(t); t.reset();

    // ── Sphere — just translated ──────────────────────────────────────
    RT_Sphere sphere_plain(&mat_red);
    t.setTranslation({-3, 0.5, -2});
    t.setScale      ({0.5, 0.5, 0.5});
    sphere_plain.setTransform(t); t.reset();

    // ── Sphere — non-uniform scale (squashed into ellipsoid) ──────────
    RT_Sphere sphere_squashed(&mat_blue);
    t.setTranslation({-1.5, 0.3, -2});
    t.setScale      ({0.6, 0.3, 0.6});   // flat ellipsoid
    sphere_squashed.setTransform(t); t.reset();

    // ── Sphere — stretched vertically ────────────────────────────────
    RT_Sphere sphere_tall(&mat_glass);
    t.setTranslation({0, 0.6, -2});
    t.setScale      ({0.3, 0.6, 0.3});   // tall ellipsoid
    sphere_tall.setTransform(t); t.reset();

    // ── Box — just translated ─────────────────────────────────────────
    RT_Box box_plain(&mat_gold);
    t.setTranslation({1.5, 0.3, -2});
    t.setScale      ({0.3, 0.3, 0.3});
    box_plain.setTransform(t); t.reset();

    // ── Box — rotated 45 degrees on Y ────────────────────────────────
    RT_Box box_rotated(&mat_silver);
    t.setTranslation({3.0, 0.3, -2});
    t.setRotation   ({0, 45, 0});
    t.setScale      ({0.3, 0.3, 0.3});
    box_rotated.setTransform(t); t.reset();

    // ── Box — rotated on multiple axes ───────────────────────────────
    RT_Box box_tilted(&mat_red);
    t.setTranslation({-3.0, 0.4, -3.5});
    t.setRotation   ({30, 45, 0});
    t.setScale      ({0.25, 0.4, 0.25});
    box_tilted.setTransform(t); t.reset();

    // ── Cylinder — upright ───────────────────────────────────────────
    RT_Cylinder cyl_upright(true, &mat_green);
    t.setTranslation({-1.0, 0.5, -3.5});
    t.setScale      ({0.2, 0.5, 0.2});
    cyl_upright.setTransform(t); t.reset();

    // ── Cylinder — rotated 90 on Z (lying on its side) ───────────────
    RT_Cylinder cyl_sideways(true, &mat_blue);
    t.setTranslation({0.5, 0.2, -3.5});
    t.setRotation   ({0, 0, 90});
    t.setScale      ({0.2, 0.5, 0.2});
    cyl_sideways.setTransform(t); t.reset();

    // ── Cylinder — tilted ────────────────────────────────────────────
    RT_Cylinder cyl_tilted(true, &mat_gold);
    t.setTranslation({2.0, 0.5, -3.5});
    t.setRotation   ({0, 0, 30});
    t.setScale      ({0.15, 0.6, 0.15});
    cyl_tilted.setTransform(t); t.reset();

    // ── Cone — upright ───────────────────────────────────────────────
    RT_Cone cone_upright(true, &mat_red);
    t.setTranslation({-2.5, 1.0, -4.0});
    t.setScale      ({0.3, 1.0, 0.3});
    cone_upright.setTransform(t); t.reset();

    // ── Cone — flipped upside down ───────────────────────────────────
    RT_Cone cone_flipped(true, &mat_silver);
    t.setTranslation({-0.5, 0.8, -4.5});
    t.setRotation   ({180, 0, 0});   // flip on X
    t.setScale      ({0.3, 0.8, 0.3});
    cone_flipped.setTransform(t); t.reset();

    // ── Cone — tilted ────────────────────────────────────────────────
    RT_Cone cone_tilted(true, &mat_green);
    t.setTranslation({1.5, 0.8, -4.5});
    t.setRotation   ({0, 0, -30});
    t.setScale      ({0.25, 0.8, 0.25});
    cone_tilted.setTransform(t); t.reset();

    // ── Scene ─────────────────────────────────────────────────────────
    RT_ObjectList scene;
    scene.add(&ground);
    scene.add(&back_wall);
    scene.add(&sphere_plain);
    scene.add(&sphere_squashed);
    scene.add(&sphere_tall);
    scene.add(&box_plain);
    scene.add(&box_rotated);
    scene.add(&box_tilted);
    scene.add(&cyl_upright);
    scene.add(&cyl_sideways);
    scene.add(&cyl_tilted);
    scene.add(&cone_upright);
    scene.add(&cone_flipped);
    scene.add(&cone_tilted);

    RT_Renderer renderer(800, 16.0/9.0, 100, 20);
    renderer.setCamera(&cam);
    renderer.setScene(&scene);
    renderer.setBackground(Color(0.5, 0.7, 1.0));
    renderer.render();
    renderer.writePPM(EXAMPLE_OUTPUT_DIR "transforms_showcase.ppm");

    std::ifstream file(EXAMPLE_OUTPUT_DIR "transforms_showcase.ppm");
    assert(file.good());
    std::cout << "transforms_showcase.ppm generated successfully\n";
    return 0;
}