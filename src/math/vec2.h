#ifndef VEC2_H
#define VEC2_H

#include <cassert>
#include <cmath>

template<typename T>
class Vec2 {
    public:
        T x, y;

        Vec2(): x(0), y(0) {}
        Vec2(T _x, T _y): x(_x), y(_y) {}

        T* data() { return &x; }
        const T* data() const { return &x; }
        
        T operator[](int i) const {
            assert(i >= 0 && i <= 1);
            return (&x)[i];
        }

        T& operator[](int i) {
            assert(i >= 0 && i <= 1);
            return (&x)[i];
        }

        Vec2<T> operator+(const Vec2<T>& v) const {
            return Vec2<T>(x + v.x, y + v.y);
        }

        Vec2<T>& operator+=(const Vec2<T>& v) {
            x += v.x;
            y += v.y;

            return *this; 
        }

        Vec2<T> operator*(T scalar) const {
            return Vec2<T>(x * scalar, y * scalar);
        }

        Vec2<T>& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;

            return *this;
        }

        Vec2<T> operator/(T scalar) const {
            assert(scalar != 0);
            float inv = (float)1 / scalar;
            return Vec2<T>(x * inv, y * inv);
        }

        Vec2<T>& operator/=(T scalar){
            assert(scalar != 0);
            double inv = (double)1 / scalar;
            x *= inv; y *= inv;
            return *this;
        }

        Vec2<T> operator-(const Vec2<T>& v) const {
            return Vec2(x - v.x, y - v.y);
        }

        Vec2<T>& operator-=(const Vec2<T>& v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }
        
        Vec2<T> operator-() const {
            return Vec2<T>(-x, -y);
        }

        bool operator==(const Vec2<T>& v) const {
            return x == v.x && y == v.y;
        }

        bool operator!=(const Vec2<T>& v) const {
            return !(*this == v);
        }

        T length() const {
            return std::sqrt(x*x + y*y);
        }

        T lengthSquared() const {
            return x*x + y*y;
        }
};

template <typename T>
Vec2<T> operator*(T scalar, const Vec2<T>& v){
    return Vec2<T>(v.x * scalar, v.y * scalar);
}

template <typename T> 
inline Vec2<T> abs(const Vec2<T>& v){
    return Vec2<T>(std::abs(v.x), std::abs(v.y));
} 

template <typename T> 
inline T dot(const Vec2<T>& v1, const Vec2<T>& v2){
    return ( v1.x * v2.x + v1.y * v2.y);
}

template <typename T>
inline Vec2<T> normalize(const Vec2<T>& v) {
    T len = v.length();
    assert (len != 0);
    return v / len;
}

template <typename T>
T distance(const Vec2<T>& a, const Vec2<T>& b){
    return (a - b).length();
}

#endif