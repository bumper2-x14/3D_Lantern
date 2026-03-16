#ifndef POINT3_H
#define POINT3_H

#include <cassert>
#include "vec3.h"

#include "utility"

template <typename T>
class Point3 {
    public:
        T x, y, z;

        Point3() : x(0), y(0), z(0) {}
        Point3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

        T operator[](int i) const {
            assert(i >= 0 && i < 3);
            return (&x)[i];
        }

        T& operator[](int i) {
            assert(i >= 0 && i < 3);
            return (&x)[i];
        }

        // POINT + VECTOR = POINT
        Point3<T> operator+(const Vec3<T>& v) const {
            return Point3<T>(x + v.x, y + v.y, z + v.z);
        }

        Point3<T>& operator+=(const Vec3<T>& v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        // POINT - VECTOR = POINT
        Point3<T> operator-(const Vec3<T>& v) const {
            return Point3<T>(x - v.x, y - v.y, z - v.z);
        }

        Point3<T>& operator-=(const Vec3<T>& v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        // POINT - POINT = VECTOR
        Vec3<T> operator-(const Point3<T>& p) const {
            return Vec3<T>(x - p.x, y - p.y, z - p.z);
        }

        // COMPARISON
        bool operator==(const Point3<T>& p) const {
            return x == p.x && y == p.y && z == p.z;
        }

        bool operator!=(const Point3<T>& p) const {
            return !(*this == p);
        }
};

using Point3f = Point3<float>;
using Point3d = Point3<double>; 

template <typename T>
inline Point3<T> random(T min_r, T max_r) {
    return Point3<T>(randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r));
}

#endif