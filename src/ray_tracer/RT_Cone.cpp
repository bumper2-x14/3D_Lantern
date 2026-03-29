#include "RT_Cone.h"
#include "RT_Disk.h"


RT_Cone::RT_Cone(bool _capped, RT_Material* _material) : capped(_capped), material(_material) {}


bool RT_Cone::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    Mat4d inv      = getInverse();
    Rayd local_ray = transformRay(ray, inv);

    // unit cone: apex=(0,0,0), k=tan(45)=1, y=-1 to 0
    // equation: x^2 + z^2 - y^2 = 0
    Vec3d oc = local_ray.getOrigin() - Point3d(0, 0, 0);
    Vec3d d  = local_ray.getDirection();

    double a = d.x*d.x + d.z*d.z - d.y*d.y;
    double b = 2.0 * (oc.x*d.x + oc.z*d.z - oc.y*d.y);
    double c = oc.x*oc.x + oc.z*oc.z - oc.y*oc.y;

    double discriminant = b*b - 4.0*a*c;
    bool   hit_anything = false;
    RT_Record closest   = rec;
    Intervald range     = t_interval;

    if (discriminant >= 0.0 && std::fabs(a) > 1e-10) {
        double sqrt_d = std::sqrt(discriminant);

        for (double sign : {-1.0, 1.0}) {
            double t = (-b + sign * sqrt_d) / (2.0 * a);
            if (!range.contains(t)) continue;

            Point3d localP = local_ray.at(t);
            if (localP.y < -1.0 || localP.y > 0.0) continue;  // y=-1 to 0

            // Gradient of the cone implicit: normal has a Y component = -k^2 * py
            double r = std::sqrt(localP.x*localP.x + localP.z*localP.z);
            if (r < 1e-10) continue;

            // cone normal in object space
            Vec3d local_normal(localP.x / r, -localP.y / r, localP.z / r);
            local_normal = normalize(local_normal); 

            closest.t = t;
            closest.p = getMatrix() * localP;
            closest.material = material;
            closest.setNormal(ray, transformNormal(local_normal, inv));

            double theta = std::atan2(localP.z, localP.x);
            closest.uv.x = (theta + M_PI) / (2.0 * M_PI);
            closest.uv.y = (localP.y + 1.0);  // remap [-1,0] to [0,1]

            range = Intervald(t_interval.min, t);
            hit_anything = true;
        }
    }

    // Base cap
    if (capped) {
        RT_Disk cap(Point3d(0, -1, 0), Vec3d(0, -1, 0), 1.0, 0.0, material);
        RT_Record cap_rec;
        if (cap.rayIntersect(local_ray, range, cap_rec)) {
            Point3d localP   = local_ray.at(cap_rec.t);
            cap_rec.p        = getMatrix() * localP;
            cap_rec.normal   = transformNormal(cap_rec.normal, inv);
            range            = Intervald(t_interval.min, cap_rec.t);
            closest          = cap_rec;
            hit_anything     = true;
        }
    }

    if (hit_anything) rec = closest;
    return hit_anything;
}
