#include "RT_Cylinder.h"
#include "RT_Disk.h"
#include <cmath>

RT_Cylinder::RT_Cylinder(bool _capped, RT_Material* _material) : capped(_capped), material(_material) {}

bool RT_Cylinder::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    Mat4d inv = getInverse();
    Rayd local_ray = transformRay(ray, inv);

    Vec3d oc = local_ray.getOrigin() - Point3d(0, 0, 0);

    double dir_x = local_ray.getDirection().x;
    double dir_z = local_ray.getDirection().z;
    double a = dir_x * dir_x + dir_z * dir_z;
    double b = 2.0 * (oc.x * dir_x + oc.z * dir_z);
    double c = oc.x * oc.x + oc.z * oc.z - 1.0;

    double discriminant = b * b - 4.0* a * c;
    bool   hit_anything = false;
    RT_Record closest   = rec;
    Intervald range     = t_interval;

    if (discriminant >= 0.0 && std::fabs(a) > 1e-10) {
        double sqrt_d = std::sqrt(discriminant);

        for (double sign : {-1.0, 1.0}) {
            double t = (-b + sign * sqrt_d) / (2.0 * a);
            if (!range.contains(t)) continue;

            Point3d localP = local_ray.at(t);

            if (localP.y < -1.0 || localP.y > 1.0) continue;

            // radial normal in object space
            Vec3d local_normal(localP.x, 0.0, localP.z);
            Vec3d world_normal = transformNormal(local_normal, inv);

            closest.t = t;
            closest.p = getMatrix() * localP;;
            closest.material = material;
            closest.setNormal(ray, world_normal);

            double theta     = std::atan2(localP.z, localP.x);
            closest.uv.x     = (theta + M_PI) / (2.0 * M_PI);
            closest.uv.y     = (localP.y + 1.0) / 2.0;  // remap [-1,1] to [0,1]

            range = Intervald(t_interval.min, t);
            hit_anything = true;
        }
    }

    // End caps
    if (capped) {
            for (double cap_y : {-1.0, 1.0}) {
            Vec3d   cap_normal(0, cap_y < 0 ? -1.0 : 1.0, 0);
            Point3d cap_center(0, cap_y, 0);

            RT_Disk cap(cap_center, cap_normal, 1.0, 0.0, material);
            RT_Record cap_rec;
            if (cap.rayIntersect(local_ray, range, cap_rec)) {  // local_ray ✓
                Point3d localP = local_ray.at(cap_rec.t);
                cap_rec.p = getMatrix() * localP;
                cap_rec.normal = transformNormal(cap_rec.normal, inv);
                range = Intervald(t_interval.min, cap_rec.t);
                closest = cap_rec;
                hit_anything = true;
            }
        }
    }

    if (hit_anything) rec = closest;
    return hit_anything;
}