#ifndef RT_RECORD_H
#define RT_RECORD_H

#include "math/ray.h"
#include "math/point3.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "RT_Material.h"

struct RT_Record {
    Point3d p; // Intersection point of ray-object
    Vec3d normal; 
    RT_Material* material; 
    double t; // 
    bool front_face;
    Vec2d uv; // Texture coordinate

    void setNormal(const Rayd& ray, const Vec3d& outward_normal){
        front_face = dot(ray.getDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

#endif