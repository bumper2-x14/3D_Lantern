#ifndef VEC4_H
#define VEC4_H

#include <cassert>
#include <cmath>

template<typename T>
class Vec4 {
public:
    T x, y, z, w;

    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(T _x, T _y, T _z, T _w = T(1)) : x(_x), y(_y), z(_z), w(_w) {}

    T& operator[](int i) {
        assert(i >= 0 && i < 4);
        return (&x)[i];
    }

    const T& operator[](int i) const {
        assert(i >= 0 && i < 4);
        return (&x)[i];
    }

    T* data() { return &x; }
    const T* data() const { return &x; }

    Vec4<T> operator+(const Vec4<T>& v) const {
        return Vec4<T>(x+v.x, y+v.y, z+v.z, w+v.w); 
    }

    Vec4<T>& operator+=(const Vec4<T>& v) {
        x+=v.x; y+=v.y; z+=v.z; w+=v.w; 
        return *this; 
    }

    Vec4<T> operator-(const Vec4<T>& v) const {
        return Vec4<T>(x-v.x, y-v.y, z-v.z, w-v.w); 
    }

    Vec4<T>& operator-=(const Vec4<T>& v) {
        x-=v.x; y-=v.y; z-=v.z; w-=v.w; 
        return *this; 
    }

    Vec4<T> operator*(T scalar) const {
        return Vec4<T>(x*scalar, y*scalar, z*scalar, w*scalar); 
    }

    Vec4<T>& operator*=(T scalar) {
        x*=scalar; y*=scalar; z*=scalar; w*=scalar; return *this; 
    }

    Vec4<T> operator/(T scalar) const {
        assert(scalar != 0); 
        T inv = T(1)/scalar; 
        return *this * inv;
    }

    Vec4<T>& operator/=(T scalar) {
        assert(scalar != 0); 
        T inv = T(1)/scalar; 
        *this *= inv; return *this; 
    }

    Vec4<T> operator-() const {
        return Vec4<T>(-x,-y,-z,-w); 
    }

    bool operator==(const Vec4<T>& v) const {
        return x==v.x && y==v.y && z==v.z && w==v.w; 
    }

    bool operator!=(const Vec4<T>& v) const {
        return !(*this == v); 
    }

    T length() const {
        return std::sqrt(x*x + y*y + z*z + w*w); 
    }

    T lengthSquared() const {
        return x*x + y*y + z*z + w*w; 
    }
};

template <typename T> 
inline T dot(const Vec4<T>& v1, const Vec4<T>& v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w); 
}

template <typename T>
inline Vec4<T> normalize(const Vec4<T>& v) {
    T len = v.length();
    assert (len != 0);
    return v / len;
}

#endif