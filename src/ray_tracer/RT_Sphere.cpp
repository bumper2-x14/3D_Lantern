#include <iostream>
#include <cassert>
#include "RT_Sphere.h"

RT_Sphere::RT_Sphere(RT_Material* _material): material(_material) {}

bool RT_Sphere::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    Mat4d inv = getInverse();
    Rayd local_ray = transformRay(ray, inv);

    Vec3d oc = local_ray.getOrigin() - Point3d(0, 0, 0);  // Calculate the vector from ray origin to the center of the sphere
    
    // Calculate the discriminant of ray-sphere intersection equation
    double a = local_ray.getDirection().lengthSquared();
    double h = dot(local_ray.getDirection(), oc);
    double c = oc.lengthSquared() - 1.0;
    double discriminant = h*h - a*c;
    
    if (discriminant < 0) return false;

    // Calculate square root of discriminant
    double sqrt_disc = std::sqrt(discriminant);

    double root = (-h - sqrt_disc) / a;
    if (!t_interval.surrounds(root)) {
        root = (-h + sqrt_disc) / a;
        if (!t_interval.surrounds(root)){
            return false;
        }
    }

    Point3d localP = local_ray.at(root);
    double theta = std::acos(-localP.y);
    double phi   = std::atan2(-localP.z, localP.x) + M_PI;
    rec.uv.x = phi   / (2.0 * M_PI);  // u in [0,1]
    rec.uv.y = theta / M_PI;          // v in [0,1]

    rec.t = root;
    rec.p = getMatrix() * localP;
    Vec3d world_normal = transformNormal((Vec3d)localP, inv);
    rec.setNormal(ray, world_normal); 
    rec.material = material;

    return true;
}

/*
void RT_Sphere::regressionTest() {

    // Minimal stub — satisfies RT_Sphere constructor without pulling in RT_Lambertian
    struct NullMaterial : public RT_Material {
        bool rayScatter(const Rayd&, const RT_Record&, Color&, Rayd&) const override {
            return false;
        }
    };
    NullMaterial mat;

    // Test constructor
    RT_Sphere sphere(Point3d(0, 0, -1), 0.5, &mat);
    assert(sphere.center == Point3d(0, 0, -1));
    assert(sphere.radius == 0.5);

    // Test direct hit through center
    Rayd ray(Point3d(0, 0, 0), Vec3d(0, 0, -1));
    std::cout << "Origin: "
              << ray.getOrigin().x << ", "
              << ray.getOrigin().y << ", "
              << ray.getOrigin().z << "\n";
    std::cout << "Direction: "
              << ray.getDirection().x << ", "
              << ray.getDirection().y << ", "
              << ray.getDirection().z << "\n";

    RT_Record rec;
    bool hit = sphere.rayIntersect(ray, Intervald(0.001, 100.0), rec);
    assert(hit);
    assert(rec.t > 0);
    assert(rec.p == ray.at(rec.t));
    assert(std::abs(rec.normal.length() - 1.0) < 1e-6);

    // Test normal points outward on front face
    assert(rec.front_face);
    assert(dot(rec.normal, ray.getDirection()) < 0);

    // Test miss
    Rayd miss_ray(Point3d(0, 0, 0), Vec3d(1, 0, 0));
    RT_Record miss_rec;
    bool miss = sphere.rayIntersect(miss_ray, Intervald(0.001, 100.0), miss_rec);
    assert(!miss);

    // Test hit behind ray is rejected
    Rayd behind_ray(Point3d(0, 0, 0), Vec3d(0, 0, 1));
    RT_Record behind_rec;
    bool behind = sphere.rayIntersect(behind_ray, Intervald(0.001, 100.0), behind_rec);
    assert(!behind);

    // Test interval rejection
    Rayd close_ray(Point3d(0, 0, 0), Vec3d(0, 0, -1));
    RT_Record close_rec;
    bool too_far = sphere.rayIntersect(close_ray, Intervald(0.001, 0.1), close_rec);
    assert(!too_far);

    // Test ray from inside sphere — should hit back face
    Rayd inside_ray(Point3d(0, 0, -1), Vec3d(0, 0, -1));
    RT_Record inside_rec;
    bool inside = sphere.rayIntersect(inside_ray, Intervald(0.001, 100.0), inside_rec);
    assert(inside);
    assert(!inside_rec.front_face);

    std::cout << "All RT_Sphere tests passed successfully\n";
}
*/