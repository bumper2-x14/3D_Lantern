#include "RT_Cone.h"
#include "RT_Disk.h"


RT_Cone::RT_Cone(const Point3d& _apex, double _half_angle, double _y_min, double _y_max,
                    bool _capped, RT_Material* _material) :
    apex(_apex), half_angle(_half_angle),
    y_min(_y_min), y_max(_y_max),
    capped(_capped), material(_material) {}


bool RT_Cone::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    double k  = std::tan(half_angle);
    double k2 = k * k;

    Vec3d oc = ray.getOrigin() - apex;
    Vec3d d  = ray.getDirection();

    double a = d.x*d.x + d.z*d.z - k2 * d.y*d.y;
    double b = 2.0 * (oc.x*d.x + oc.z*d.z - k2 * oc.y*d.y);
    double c = oc.x*oc.x + oc.z*oc.z - k2 * oc.y*oc.y;

    double discriminant = b*b - 4.0*a*c;
    bool   hit_anything = false;
    RT_Record closest   = rec;
    Intervald range     = t_interval;

    if (discriminant >= 0.0 && std::fabs(a) > 1e-10) {
        double sqrt_d = std::sqrt(discriminant);

        for (double sign : {-1.0, 1.0}) {
            double t = (-b + sign * sqrt_d) / (2.0 * a);
            if (!range.contains(t)) continue;

            Point3d p  = ray.at(t);
            double  py = p.y - apex.y;   // local Y from apex

            if (py < y_min || py > y_max) continue;

            // Gradient of the cone implicit: normal has a Y component = -k^2 * py
            double r = std::sqrt((p.x-apex.x)*(p.x-apex.x)
                               + (p.z-apex.z)*(p.z-apex.z));
            Vec3d outward_normal(
                (p.x - apex.x) / r,
                -k * std::cos(half_angle),
                (p.z - apex.z) / r);
            outward_normal = normalize(outward_normal);

            closest.t        = t;
            closest.p        = p;
            closest.material = material;
            closest.setNormal(ray, outward_normal);

            
            double theta = std::atan2(p.z - apex.z, p.x - apex.x);
            closest.uv.x = (theta + M_PI) / (2.0 * M_PI);
            closest.uv.y = (py - y_min) / (y_max - y_min);
            
            range        = Intervald(t_interval.min, t);
            hit_anything = true;
        }
    }

    // Base cap
    if (capped) {
        double cap_y  = y_max;   // base is at y_max from apex
        double cap_r  = std::fabs(cap_y) * k;
        Point3d cap_center(apex.x, apex.y + cap_y, apex.z);
        Vec3d   cap_normal(0, cap_y >= 0 ? 1.0 : -1.0, 0);
        RT_Disk cap(cap_center, cap_normal, cap_r, 0.0, material);

        RT_Record cap_rec;
        if (cap.rayIntersect(ray, range, cap_rec)) {
            closest      = cap_rec;
            hit_anything = true;
        }
    }

    if (hit_anything) rec = closest;
    return hit_anything;
}
