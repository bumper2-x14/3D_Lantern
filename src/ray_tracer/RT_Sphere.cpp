#include "RT_Sphere.h"

RT_Sphere::RT_Sphere(const Point3d& _center, double _radius): 
    center(_center), radius(_radius) {}

bool RT_Sphere::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    Vec3d oc = center - ray.getOrigin(); // Calculate the vector from ray origin to the center of the sphere
    
    // Calculate the discriminant of ray-sphere intersection equation
    double a = ray.getDirection().lengthSquared();
    double h = dot(ray.getDirection(), oc);
    double c = oc.lengthSquared() - (radius * radius);
    double discriminant = h*h - a*c;
    
    if (discriminant < 0) return false;

    // Calculate square root of discriminant
    double sqrt_disc = std::sqrt(discriminant);

    double root = (h - sqrt_disc) / a;
    if (!t_interval.surrounds(root)) {
        root = (h + sqrt_disc) / a;
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