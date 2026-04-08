#include <iostream>
#include <cassert>
#include "RT_Sphere.h"

RT_Sphere::RT_Sphere(RT_Material* _material): material(_material) {}

void RT_Sphere::setTransform(const TRSTransformd& t) {
    RT_Object::setTransform(t);
    setBoundingBox(computeBBox(BoundingBoxd(Point3d(-1,-1,-1), Point3d(1,1,1))));
}

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
