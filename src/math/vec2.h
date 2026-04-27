#ifndef VEC2_H
#define VEC2_H

#include <cassert>
#include <cmath>

#include "utility.h"

/// @brief Simple 2D vector class.
///        Used for UV coords, screen space, etc.
template<typename T>
class Vec2 {
    public:
        T x, y; ///< vector components

        /// @brief default constructor (0,0)
        Vec2(): x(0), y(0) {}

        /// @brief constructor with values
        Vec2(T _x, T _y): x(_x), y(_y) {}

        /// @brief pointer to raw data (useful for OpenGL)
        T* data() { return &x; }

        /// @brief const pointer to raw data
        const T* data() const { return &x; }
        
        /// @brief read access by index
        T operator[](int i) const {
            assert(i >= 0 && i <= 1);
            return (&x)[i];
        }

        /// @brief write access by index
        T& operator[](int i) {
            assert(i >= 0 && i <= 1);
            return (&x)[i];
        }

        /// @brief vector addition
        Vec2<T> operator+(const Vec2<T>& v) const {
            return Vec2<T>(x + v.x, y + v.y);
        }

        /// @brief += operator
        Vec2<T>& operator+=(const Vec2<T>& v) {
            x += v.x;
            y += v.y;
            return *this; 
        }

        /// @brief scalar multiplication
        Vec2<T> operator*(T scalar) const {
            return Vec2<T>(x * scalar, y * scalar);
        }

        /// @brief *= operator
        Vec2<T>& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        /// @brief scalar division
        Vec2<T> operator/(T scalar) const {
            assert(scalar != 0);
            float inv = (float)1 / scalar;
            return Vec2<T>(x * inv, y * inv);
        }

        /// @brief /= operator
        Vec2<T>& operator/=(T scalar){
            assert(scalar != 0);
            double inv = (double)1 / scalar;
            x *= inv; y *= inv;
            return *this;
        }

        /// @brief vector subtraction
        Vec2<T> operator-(const Vec2<T>& v) const {
            return Vec2(x - v.x, y - v.y);
        }

        /// @brief -= operator
        Vec2<T>& operator-=(const Vec2<T>& v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }
        
        /// @brief negation (-v)
        Vec2<T> operator-() const {
            return Vec2<T>(-x, -y);
        }

        /// @brief equality check
        bool operator==(const Vec2<T>& v) const {
            return x == v.x && y == v.y;
        }

        /// @brief inequality check
        bool operator!=(const Vec2<T>& v) const {
            return !(*this == v);
        }

        /// @brief vector length
        T length() const {
            return std::sqrt(x*x + y*y);
        }

        /// @brief squared length (faster)
        T lengthSquared() const {
            return x*x + y*y;
        }
};

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

/// @brief scalar * vector
template <typename T>
Vec2<T> operator*(T scalar, const Vec2<T>& v){
    return Vec2<T>(v.x * scalar, v.y * scalar);
}

/// @brief absolute value of vector
template <typename T> 
inline Vec2<T> abs(const Vec2<T>& v){
    return Vec2<T>(std::abs(v.x), std::abs(v.y));
} 

/// @brief dot product
template <typename T> 
inline T dot(const Vec2<T>& v1, const Vec2<T>& v2){
    return ( v1.x * v2.x + v1.y * v2.y);
}

/// @brief normalize vector
template <typename T>
inline Vec2<T> normalize(const Vec2<T>& v) {
    T len = v.length();
    assert (len != 0);
    return v / len;
}

/// @brief distance between two points
template <typename T>
inline T distance(const Vec2<T>& a, const Vec2<T>& b){
    return (a - b).length();
}

/// @brief random vector in range
template <typename T>
inline Vec2<T> random(T min_r, T max_r) {
    return Vec2<T>(randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r));
} 

#endif