#include <iostream>
#include <cassert>
#include "RT_Sphere.h"

RT_Sphere::RT_Sphere(const Point3d& _center, double _radius): 
    center(_center), radius(_radius) {}

bool RT_Sphere::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    Vec3d oc = ray.getOrigin() - center; ; // Calculate the vector from ray origin to the center of the sphere
    
    // Calculate the discriminant of ray-sphere intersection equation
    double a = ray.getDirection().lengthSquared();
    double h = dot(ray.getDirection(), oc);
    double c = oc.lengthSquared() - (radius * radius);
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

    rec.p = ray.at(root);
    rec.t = root;
    Vec3d out_normal = (rec.p - center) / radius;
    rec.setNormal(ray, out_normal); 

    return true;
}

void RT_Sphere::regressionTest() {
    // Test constructor
    RT_Sphere sphere(Point3d(0, 0, -1), 0.5);
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
    assert(rec.p == ray.at(rec.t)); // hit point on ray
    assert(std::abs(rec.normal.length() - 1.0) < 1e-6); // normal is unit length

    // Test normal points outward on front face
    assert(rec.front_face);
    assert(dot(rec.normal, ray.getDirection()) < 0); // normal opposes ray

    // Test miss
    Rayd miss_ray(Point3d(0, 0, 0), Vec3d(1, 0, 0)); // pointing right
    RT_Record miss_rec;
    bool miss = sphere.rayIntersect(miss_ray, Intervald(0.001, 100.0), miss_rec);
    assert(!miss);

    // Test hit behind ray is rejected
    Rayd behind_ray(Point3d(0, 0, 0), Vec3d(0, 0, 1)); // pointing away
    RT_Record behind_rec;
    bool behind = sphere.rayIntersect(behind_ray, Intervald(0.001, 100.0), behind_rec);
    assert(!behind);

    // Test interval rejection
    Rayd close_ray(Point3d(0, 0, 0), Vec3d(0, 0, -1));
    RT_Record close_rec;
    bool too_far = sphere.rayIntersect(close_ray, Intervald(0.001, 0.1), close_rec);
    assert(!too_far); // sphere at t~0.5, interval max is 0.1

    // Test ray from inside sphere — should hit back face
    Rayd inside_ray(Point3d(0, 0, -1), Vec3d(0, 0, -1)); // origin at sphere center
    RT_Record inside_rec;
    bool inside = sphere.rayIntersect(inside_ray, Intervald(0.001, 100.0), inside_rec);
    assert(inside);
    assert(!inside_rec.front_face); // hitting from inside

    std::cout << "All RT_Sphere tests passed successfully\n";
}