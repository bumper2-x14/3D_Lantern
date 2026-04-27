#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "point3.h"

/// @brief Ray class used in ray tracing.
///        Defined by origin + direction (+ optional time for motion blur).
template <typename T>
class Ray {
    public:
        /// @brief Default constructor.
        Ray() {}

        /// @brief Creates a ray with origin and direction (time = 0).
        Ray(const Point3<T>& _origin, const Vec3<T>& _direction)
            : Ray(_origin, _direction, 0.0) {}
            
        /// @brief Creates a ray with origin, direction and time.
        Ray(const Point3<T>& _origin, const Vec3<T>& _direction, double _time):
            origin(_origin), direction(_direction), time(_time) {}

        // ---------- getters (const) ----------

        /// @brief Returns ray origin (read-only).
        const Point3<T>& getOrigin() const { return origin; }

        /// @brief Returns ray direction (read-only).
        const Vec3<T>& getDirection() const { return direction; }

        /// @brief Returns ray time (read-only).
        double getTime() const { return time; }

        // ---------- getters (mutable) ----------

        /// @brief Returns ray origin (modifiable).
        Point3<T>& getOrigin() { return origin; }

        /// @brief Returns ray direction (modifiable).
        Vec3<T>& getDirection() { return direction; }

        /// @brief Returns ray time (modifiable).
        double& getTime() { return time; }

        /// @brief Returns point along the ray at parameter t.
        ///        p(t) = origin + t * direction
        Point3<T> at(T t) const {
            return origin + (t * direction);
        }

    private:    
        Point3<T> origin;   ///< Starting point of the ray.
        Vec3<T> direction;  ///< Direction of the ray.
        double time;        ///< Time (used for motion blur).
};

using Rayf = Ray<float>;
using Rayd = Ray<double>;

#endif