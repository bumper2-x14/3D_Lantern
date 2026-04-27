#ifndef POINT3_H
#define POINT3_H

#include <cassert>
#include <ostream>

#include "vec3.h"
#include "utility"

/// @brief Represents a 3D point.
///        Different from Vec3 because it represents a position, not a direction.
template <typename T>
class Point3 {
    public:
        T x, y, z; ///< Point coordinates.

        /// @brief Default constructor (origin).
        Point3() : x(0), y(0), z(0) {}

        /// @brief Creates a point from x, y, z.
        Point3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

        /// @brief Returns coordinate by index.
        T operator[](int i) const {
            assert(i >= 0 && i < 3);
            return (&x)[i];
        }

        /// @brief Returns editable coordinate by index.
        T& operator[](int i) {
            assert(i >= 0 && i < 3);
            return (&x)[i];
        }

        // POINT + VECTOR = POINT

        /// @brief Moves point by a vector.
        Point3<T> operator+(const Vec3<T>& v) const {
            return Point3<T>(x + v.x, y + v.y, z + v.z);
        }

        /// @brief Moves this point by a vector.
        Point3<T>& operator+=(const Vec3<T>& v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        // POINT - VECTOR = POINT

        /// @brief Moves point backward by a vector.
        Point3<T> operator-(const Vec3<T>& v) const {
            return Point3<T>(x - v.x, y - v.y, z - v.z);
        }

        /// @brief Moves this point backward by a vector.
        Point3<T>& operator-=(const Vec3<T>& v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        // POINT - POINT = VECTOR

        /// @brief Difference between two points gives a vector.
        Vec3<T> operator-(const Point3<T>& p) const {
            return Vec3<T>(x - p.x, y - p.y, z - p.z);
        }

        /// @brief Multiplies point coordinates by an integer.
        Point3<T> operator*(int s) const {
            return Point3<T>(x*s, y*s, z*s);
        }

        // COMPARISON

        /// @brief Checks if two points are equal.
        bool operator==(const Point3<T>& p) const {
            return x == p.x && y == p.y && z == p.z;
        }

        /// @brief Checks if two points are different.
        bool operator!=(const Point3<T>& p) const {
            return !(*this == p);
        }

        /// @brief Converts point to Vec3.
        explicit operator Vec3<T>() const { return Vec3<T>(x, y, z); }
};

using Point3f = Point3<float>;
using Point3d = Point3<double>; 

/// @brief Prints a point.
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Point3<T>& v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

/// @brief Creates a random point inside [min_r, max_r].
template <typename T>
inline Point3<T> random(T min_r, T max_r) {
    return Point3<T>(randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r));
}

#endif