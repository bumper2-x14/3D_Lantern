#include "RT_Cylinder.h"
#include "RT_Disk.h"
#include <cmath>

RT_Cylinder::RT_Cylinder(const Point3d& _center, double _radius, double _y_min, double _y_max,
                bool _capped, RT_Material* _material) :
                center(_center), radius(_radius), y_min(_y_min), y_max(_y_max), capped(_capped), material(_material)                               
                {}


bool RT_Cylinder::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    Vec3d oc = ray.getOrigin() - center;

    double dir_x = ray.getDirection().x;
    double dir_z = ray.getDirection().z;
    double a = dir_x * dir_x + dir_z * dir_z;
    double b = 2.0 * (oc.x * dir_x + oc.z * dir_z);
    double c = oc.x * oc.x + oc.z * oc.z - radius * radius;

    double discriminant = b * b - 4.0* a * c;
    bool   hit_anything = false;
    RT_Record closest   = rec;
    Intervald range     = t_interval;

    if (discriminant >= 0.0 && std::fabs(a) > 1e-10) {
        double sqrt_d = std::sqrt(discriminant);

        for (double sign : {-1.0, 1.0}) {
            double t = (-b + sign * sqrt_d) / (2.0 * a);
            if (!range.contains(t)) continue;

            Point3d p  = ray.at(t);
            double  py = p.y - center.y;  // local Y

            if (py < y_min || py > y_max) continue;

            // Outward normal points radially from the axis
            Vec3d outward_normal(
                (p.x - center.x) / radius,
                0.0,
                (p.z - center.z) / radius);

            closest.t = t;
            closest.p = p;
            closest.material = material;
            closest.setNormal(ray, outward_normal);

            double theta = std::atan2(p.z - center.z,
                                      p.x - center.x);
            
            closest.uv.x = (theta + M_PI) / (2.0 * M_PI);
            closest.uv.y = (py - y_min) / (y_max - y_min);
            

            range = Intervald(t_interval.min, t);
            hit_anything = true;
        }
    }

    // End caps
    if (capped) {
        for (double cap_y : {y_min, y_max}) {
            Vec3d cap_normal(0, cap_y < 0 ? -1.0 : 1.0, 0);
            Point3d cap_center(center.x, center.y + cap_y, center.z);
            RT_Disk cap(cap_center, cap_normal, radius, 0.0, material);

            RT_Record cap_rec;
            if (cap.rayIntersect(ray, range, cap_rec)) {
                range = Intervald(t_interval.min, cap_rec.t);
                closest = cap_rec;
                hit_anything = true;
            }
        }
    }

    if (hit_anything) rec = closest;
    return hit_anything;
}