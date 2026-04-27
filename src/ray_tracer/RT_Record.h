#ifndef RT_RECORD_H
#define RT_RECORD_H

#include "math/ray.h"
#include "math/point3.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "RT_Material.h"

/// @brief Stores information about a ray-object intersection.
struct RT_Record {
    Point3d p; ///< Intersection point.
    Vec3d normal; ///< Surface normal at the hit point.
    RT_Material* material; ///< Material at the hit point.
    double t; ///< Ray parameter (distance along the ray).
    bool front_face; ///< True if the ray hits the front face.
    Vec2d uv; ///< Texture coordinates.

    /// @brief Sets the correct normal orientation (always against the ray).
    void setNormal(const Rayd& ray, const Vec3d& outward_normal){
        front_face = dot(ray.getDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

#endif