#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "point3.h"

template <typename T>
class Ray {
    public:
        Ray() {}
        Ray(const Point3<T>& _origin, const Vec3<T>& _direction): Ray(_origin, _direction, 0.0) {}
            
        Ray(const Point3<T>& _origin, const Vec3<T>& _direction, double _time):
            origin(_origin), direction(_direction), time(_time) {}

        // Return an immutable refrence to origin
        const Point3<T>& getOrigin() const { return origin; }
        // Return an immutable refrence to direction
        const Vec3<T>& getDirection() const { return direction; }
        // Return an immutable refrence to time
        double getTime() const { return time; }

        // Return a mutable refrence to origin
        Point3<T>& getOrigin() { return origin; }
        // Return a mutable refrence to direction
        Vec3<T>& getDirection() { return direction; }
        // Return a mutable refrence to time
        double& getTime() { return time; }

        // Retuen a point at space given some t
        Point3<T> at(T t) const {
            return origin + (t * direction);
        }

    private:    
        Point3<T> origin;
        Vec3<T> direction;
        double time;
};

using Rayf = Ray<float>;
using Rayd = Ray<double>;

#endif