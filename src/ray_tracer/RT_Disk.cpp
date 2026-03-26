#include "RT_Disk.h"
#include <cmath>

RT_Disk::RT_Disk(const Point3d& _center, const Vec3d& _normal, double _radius, 
                    double _inner_radius, RT_Material* _material) : 
                    center(_center), normal(_normal), radius(_radius), inner_radius(_inner_radius), 
                    material(_material) {

                    D = dot(normal, Vec3d(center.x, center.y, center.z));
}


bool RT_Disk::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    double denom = dot(normal, ray.getDirection());
    if (std::fabs(denom) < 1e-8){
        return false;   // ray parallel to disk plane
    }

    double t = (D - dot(normal, Vec3d(ray.getOrigin().x, ray.getOrigin().y, ray.getOrigin().z))) / denom;
    if (!t_interval.contains(t)){
        return false;
    }

    Point3d p = ray.at(t);
    Vec3d intersect_vec = p - center;
    double dist2 = intersect_vec.lengthSquared();

    if (dist2 > radius * radius){
        return false;
    }
    if (dist2 < inner_radius * inner_radius){
        return false;
    }

    rec.t = t;
    rec.p = p;
    rec.material = material;
    rec.setNormal(ray, normal);

    // UV: radial + angular
    double r   = std::sqrt(dist2) / radius;        // [0,1]
    double phi = std::atan2(intersect_vec.y, intersect_vec.x); // [-pi,pi]
    rec.uv.x = (phi + M_PI) / (2.0 * M_PI);
    rec.uv.y = r;

    return true;
}