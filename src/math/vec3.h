#ifndef VEC3_H
#define VEC3_H

#include <cassert>
#include <cmath>
#include <ostream>

#include "utility.h"

template <typename T>
class Vec3 {
    public:
        T x, y, z;

        Vec3(): x(0), y(0), z(0) {}
        Vec3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {}

        T* data() { return &x; }
        const T* data() const { return &x; }

        T operator[](int i) const {
            assert(i >= 0 && i <= 2);
            return (&x)[i];
        }

        T& operator[](int i) {
            assert(i >= 0 && i <= 2);
            return (&x)[i];
        }

        Vec3<T> operator+(const Vec3<T>& v) const {
            return Vec3<T>(x + v.x, y + v.y, z + v.z);
        }

        Vec3<T>& operator+=(const Vec3<T>& v) {
            x += v.x;
            y += v.y;
            z += v.z;

            return *this; 
        }

        Vec3<T> operator*(T scalar) const {
            return Vec3<T>(x * scalar, y * scalar, z * scalar);
        }


        Vec3<T>& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;

            return *this;
        }

        Vec3<T>& operator*=(const Vec3<T>& v) {
            x *= v.x;
            y *= v.y;
            z *= v.z;

            return *this;
        }

        Vec3<T> operator/(T scalar) const {
            assert(scalar != 0);
            double inv = (double)1 / scalar;
            return Vec3<T>(x * inv, y * inv, z * inv);
        }

        Vec3<T>& operator/=(T scalar){
            assert(scalar != 0);
            double inv = (double)1 / scalar;
            x *= inv; y *= inv; z *= inv;
            return *this;
        }

        Vec3<T> operator-(const Vec3<T>& v) const {
            return Vec3(x - v.x, y - v.y, z - v.z);
        }

        Vec3<T>& operator-=(const Vec3<T>& v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;

            return *this;
        }
        
        Vec3<T> operator-() const {
            return Vec3<T>(-x, -y, -z);
        }

        bool operator==(const Vec3<T>& v) const {
            return x == v.x && y == v.y && z == v.z;
        }

        bool operator!=(const Vec3<T>& v) const {
            return !(*this == v);
        }

        T length() const {
            return std::sqrt(x*x + y*y + z*z);
        }

        T lengthSquared() const {
            return x*x + y*y + z*z;
        }

        bool nearZero() const {
            auto s = 1e-8;
            return (std::abs(x) < s) && (std::abs(y) < s) && (std::abs(z) < s);
        }

        static Vec3<T> random(T min_r, T max_r) {
            return Vec3<T>(randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r));

        } 
};

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vec3<T>& v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template <typename T>
Vec3<T> operator*(T scalar, const Vec3<T>& v){
    return Vec3<T>(v.x * scalar, v.y * scalar, v.z * scalar);
}

template <typename T> 
inline Vec3<T> abs(const Vec3<T>& v){
    return Vec3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
} 

template <typename T> 
inline T dot(const Vec3<T>& v1, const Vec3<T>& v2){
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

template <typename T>
inline Vec3<T> cross(const Vec3<T>& v1, const Vec3<T>& v2){
    // A small optimaization by storing data as local variable
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    double v2x = v2.x, v2y = v2.y, v2z = v2.z;
    
    return Vec3<T>((v1y * v2z) - (v1z * v2y),
                    (v1z * v2x) - (v1x * v2z),
                    (v1x * v2y) - (v1y * v2x));
}

template <typename T>
inline Vec3<T> normalize(const Vec3<T>& v) {
    T len = v.length();
    //assert (len != 0);
    if (len == 0) return Vec3<T>(0, 1, 0);
    return v / len;
}

template <typename T>
T distance(const Vec3<T>& a, const Vec3<T>& b){
    return (a - b).length();
}

template <typename T>
static inline Vec3<T> random(T min_r, T max_r) {
    return Vec3<T>(randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r), randomizer<T>(min_r, max_r));
} 

template <typename T>
inline Vec3<T> UnitDiskRandom() {
    while(true) {
        Vec3<T> p = Vec3<T>(randomizer<T>(-1, 1), randomizer<T>(-1, 1), 0);
        if (p.lengthSquared() < 1)
            return p;
    }
}

template <typename T>
inline Vec3<T> randomUnitVector() {
    while (true) {
        Vec3<T> p(randomizer<T>(T(-1), T(1)),
                  randomizer<T>(T(-1), T(1)),
                  randomizer<T>(T(-1), T(1)));
        T lensq = p.lengthSquared();
        if (T(1e-160) < lensq && lensq <= T(1))
            return p / std::sqrt(lensq);
    }
}

template <typename T>
inline Vec3<T> randomOnHemisphere(const Vec3<T>& normal) {
    Vec3<T> unit_vec = randomUnitVector<T>();
    if (dot(unit_vec, normal) > 0.0) 
        return unit_vec;
    else
        return -unit_vec;
}

template <typename T>
inline Vec3<T> reflect(const Vec3<T>& v, const Vec3<T>& normal) {
    return v - 2 * dot(v, normal) * normal;
}

template <typename T>
inline Vec3<T> refract (const Vec3<T>& uv, const Vec3<T>& n, T eta_ratio) {
    T cos_theta = std::min(dot(-uv, n), T(1));
    Vec3<T> r_perp = eta_ratio * (uv + cos_theta * n);
    Vec3<T> r_para = -std::sqrt(std::abs(T(1) - r_perp.lengthSquared())) * n;
    return r_perp + r_para;
}

#endif
