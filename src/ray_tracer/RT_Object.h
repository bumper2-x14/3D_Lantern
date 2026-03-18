#ifndef RT_OBJECT_H
#define RT_OBJECT_H

#include "math/ray.h"
#include "math/interval.h"
#include "math/point3.h"
#include "math/vec3.h"

struct RT_Record {
    Point3d p;
    Vec3d normal;
    double t;
    bool front_face;

    void setNormal(const Rayd& ray, const Vec3d& outward_normal){
        front_face = dot(ray.getDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class RT_Object {
    public:
        virtual ~RT_Object() = default;    

        virtual bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const = 0;
};

#endif
