#ifndef VEC4_H
#define VEC4_H

#include <cassert>
#include <iostream>
#include <chrono>
#include <cmath>

#include "utility"

/// @brief Simple 4D vector class.
///        Mainly used for homogeneous coordinates and matrix operations.
template<typename T>
class Vec4 {
    public:
        T x, y, z, w; ///< Vector components.

        /// @brief Default constructor (0,0,0,0).
        Vec4() : x(0), y(0), z(0), w(0) {}

        /// @brief Creates a Vec4 from values.
        Vec4(T _x, T _y, T _z, T _w = T(1)) : x(_x), y(_y), z(_z), w(_w) {}

        /// @brief Write access by index.
        T& operator[](int i) {
            assert(i >= 0 && i < 4);
            return (&x)[i];
        }

        /// @brief Read access by index.
        const T& operator[](int i) const {
            assert(i >= 0 && i < 4);
            return (&x)[i];
        }

        /// @brief Returns pointer to raw data.
        T* data() { return &x; }

        /// @brief Returns const pointer to raw data.
        const T* data() const { return &x; }

        /// @brief Vector addition.
        Vec4<T> operator+(const Vec4<T>& v) const {
            return Vec4<T>(x+v.x, y+v.y, z+v.z, w+v.w); 
        }

        /// @brief Add-assign.
        Vec4<T>& operator+=(const Vec4<T>& v) {
            x+=v.x; y+=v.y; z+=v.z; w+=v.w; 
            return *this; 
        }

        /// @brief Vector subtraction.
        Vec4<T> operator-(const Vec4<T>& v) const {
            return Vec4<T>(x-v.x, y-v.y, z-v.z, w-v.w); 
        }

        /// @brief Subtract-assign.
        Vec4<T>& operator-=(const Vec4<T>& v) {
            x-=v.x; y-=v.y; z-=v.z; w-=v.w; 
            return *this; 
        }

        /// @brief Scalar multiplication.
        Vec4<T> operator*(T scalar) const {
            return Vec4<T>(x*scalar, y*scalar, z*scalar, w*scalar); 
        }

        /// @brief Multiply-assign.
        Vec4<T>& operator*=(T scalar) {
            x*=scalar; y*=scalar; z*=scalar; w*=scalar; return *this; 
        }

        /// @brief Scalar division.
        Vec4<T> operator/(T scalar) const {
            assert(scalar != 0); 
            T inv = T(1)/scalar; 
            return *this * inv;
        }

        /// @brief Divide-assign.
        Vec4<T>& operator/=(T scalar) {
            assert(scalar != 0); 
            T inv = T(1)/scalar; 
            *this *= inv; return *this; 
        }

        /// @brief Negation (-v).
        Vec4<T> operator-() const {
            return Vec4<T>(-x,-y,-z,-w); 
        }

        /// @brief Equality check.
        bool operator==(const Vec4<T>& v) const {
            return x==v.x && y==v.y && z==v.z && w==v.w; 
        }

        /// @brief Inequality check.
        bool operator!=(const Vec4<T>& v) const {
            return !(*this == v); 
        }

        /// @brief Vector length.
        T length() const {
            return std::sqrt(x*x + y*y + z*z + w*w); 
        }

        /// @brief Squared length.
        T lengthSquared() const {
            return x*x + y*y + z*z + w*w; 
        }
};

using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;

/// @brief Dot product.
template <typename T> 
inline T dot(const Vec4<T>& v1, const Vec4<T>& v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w); 
}

/// @brief Returns normalized vector.
template <typename T>
inline Vec4<T> normalize(const Vec4<T>& v) {
    T len = v.length();
    assert (len != 0);
    return v / len;
}

/// @brief Creates a random Vec4 in range.
template <typename T>
inline Vec4<T> random(T min_r, T max_r) {
    return Vec4<T>(randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r), 
                    randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r));
}

#endif