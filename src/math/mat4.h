#ifndef MAT4_H
#define MAT4_H

#include <cmath>
#include <cassert>

#include "vec3.h"
#include "vec4.h"


template <typename T>
class Mat4 {
    public:
        Mat4() {
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    matrix[i][j] = (i == j) ? (T)1 : (T)0;
                }
            }
        }

        Mat4(const T (&_m)[4][4]) {
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++){
                    matrix[i][j] = _m[i][j];
                }
            }
        }

        Mat4(T m00, T m01, T m02, T m03,
            T m10, T m11, T m12, T m13,
            T m20, T m21, T m22, T m23,
            T m30, T m31, T m32, T m33) {
            matrix[0][0]=m00; matrix[0][1]=m01; matrix[0][2]=m02; matrix[0][3]=m03;
            matrix[1][0]=m10; matrix[1][1]=m11; matrix[1][2]=m12; matrix[1][3]=m13;
            matrix[2][0]=m20; matrix[2][1]=m21; matrix[2][2]=m22; matrix[2][3]=m23;
            matrix[3][0]=m30; matrix[3][1]=m31; matrix[3][2]=m32; matrix[3][3]=m33;
        }

        Mat4(const T m[16]) {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    matrix[i][j] = m[i * 4 + j];
        }

        Mat4(Vec4<T> c0, Vec4<T> c1, Vec4<T> c2, Vec4<T> c3) {
            matrix[0][0]=c0.x; matrix[1][0]=c0.y; matrix[2][0]=c0.z; matrix[3][0]=c0.w;
            matrix[0][1]=c1.x; matrix[1][1]=c1.y; matrix[2][1]=c1.z; matrix[3][1]=c1.w;
            matrix[0][2]=c2.x; matrix[1][2]=c2.y; matrix[2][2]=c2.z; matrix[3][2]=c2.w;
            matrix[0][3]=c3.x; matrix[1][3]=c3.y; matrix[2][3]=c3.z; matrix[3][3]=c3.w;
        }
        
        T operator()(int row, int col) const {
            return matrix[col][row];
        }

        T& operator()(int row, int col) {
            return matrix[col][row];
        }

        Mat4<T>& operator=(const Mat4<T>& _m) {
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++){
                    matrix[i][j] = _m.matrix[i][j];
                }
            }
            return *this;
        }

        Vec4<T> operator*(const Vec4<T>& v) const {
            return Vec4<T>(
                (*this)(0,0)*v.x + (*this)(0,1)*v.y + (*this)(0,2)*v.z + (*this)(0,3)*v.w,
                (*this)(1,0)*v.x + (*this)(1,1)*v.y + (*this)(1,2)*v.z + (*this)(1,3)*v.w,
                (*this)(2,0)*v.x + (*this)(2,1)*v.y + (*this)(2,2)*v.z + (*this)(2,3)*v.w,
                (*this)(3,0)*v.x + (*this)(3,1)*v.y + (*this)(3,2)*v.z + (*this)(3,3)*v.w
            );
        }

        Vec3<T> operator*(const Vec3<T>& v) const {
            return Vec3<T>(
                (*this)(0,0)*v.x + (*this)(0,1)*v.y + (*this)(0,2)*v.z + (*this)(0,3),
                (*this)(1,0)*v.x + (*this)(1,1)*v.y + (*this)(1,2)*v.z + (*this)(1,3),
                (*this)(2,0)*v.x + (*this)(2,1)*v.y + (*this)(2,2)*v.z + (*this)(2,3)
            );
        }

        Vec3<T> project(const Vec4<T>& v) const {
            Vec4<T> r = (*this) * v;
            return Vec3<T>(r.x / r.w, r.y / r.w, r.z / r.w);
        }

        const T* data() const { return &matrix[0][0]; }
        
        T* data() { return &matrix[0][0]; }

        Mat4 operator*(const Mat4& b) const {
            Mat4 r = Mat4::zero();
            for (int i = 0; i < 4; i++)
                for (int k = 0; k < 4; k++)
                    for (int j = 0; j < 4; j++)
                        r(i,j) += (*this)(i,k) * b(k,j);
            return r;
        }

        Mat4 transpose() const {
            Mat4 r = Mat4::zero();
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    r(i, j) = (*this)(j, i);
            return r;
        }

        Mat4 inverse() const {
            T s0 = (*this)(0,0)*(*this)(1,1) - (*this)(1,0)*(*this)(0,1);
            T s1 = (*this)(0,0)*(*this)(2,1) - (*this)(2,0)*(*this)(0,1);
            T s2 = (*this)(0,0)*(*this)(3,1) - (*this)(3,0)*(*this)(0,1);
            T s3 = (*this)(1,0)*(*this)(2,1) - (*this)(2,0)*(*this)(1,1);
            T s4 = (*this)(1,0)*(*this)(3,1) - (*this)(3,0)*(*this)(1,1);
            T s5 = (*this)(2,0)*(*this)(3,1) - (*this)(3,0)*(*this)(2,1);

            T c5 = (*this)(2,2)*(*this)(3,3) - (*this)(3,2)*(*this)(2,3);
            T c4 = (*this)(1,2)*(*this)(3,3) - (*this)(3,2)*(*this)(1,3);
            T c3 = (*this)(1,2)*(*this)(2,3) - (*this)(2,2)*(*this)(1,3);
            T c2 = (*this)(0,2)*(*this)(3,3) - (*this)(3,2)*(*this)(0,3);
            T c1 = (*this)(0,2)*(*this)(2,3) - (*this)(2,2)*(*this)(0,3);
            T c0 = (*this)(0,2)*(*this)(1,3) - (*this)(1,2)*(*this)(0,3);

            T det = s0*c5 - s1*c4 + s2*c3 + s3*c2 - s4*c1 + s5*c0;
            assert(std::abs(det) > T(1e-8) && "Matrix is not invertible");
            T inv = T(1) / det;

            Mat4 r = Mat4::zero();
            r(0,0) = ( (*this)(1,1)*c5 - (*this)(2,1)*c4 + (*this)(3,1)*c3) * inv;
            r(0,1) = (-(*this)(0,1)*c5 + (*this)(2,1)*c2 - (*this)(3,1)*c1) * inv;
            r(0,2) = ( (*this)(0,1)*c4 - (*this)(1,1)*c2 + (*this)(3,1)*c0) * inv;
            r(0,3) = (-(*this)(0,1)*c3 + (*this)(1,1)*c1 - (*this)(2,1)*c0) * inv;

            r(1,0) = (-(*this)(1,0)*c5 + (*this)(2,0)*c4 - (*this)(3,0)*c3) * inv;
            r(1,1) = ( (*this)(0,0)*c5 - (*this)(2,0)*c2 + (*this)(3,0)*c1) * inv;
            r(1,2) = (-(*this)(0,0)*c4 + (*this)(1,0)*c2 - (*this)(3,0)*c0) * inv;
            r(1,3) = ( (*this)(0,0)*c3 - (*this)(1,0)*c1 + (*this)(2,0)*c0) * inv;

            r(2,0) = ( s3*(*this)(3,3) - s4*(*this)(2,3) + s5*(*this)(1,3)) * inv;
            r(2,1) = (-s3*(*this)(3,2) + s4*(*this)(2,2) - s5*(*this)(1,2)) * inv;
            r(2,2) = ( s0*(*this)(3,3) - s2*(*this)(1,3) + s4*(*this)(0,3)) * inv;
            r(2,3) = (-s0*(*this)(3,2) + s2*(*this)(1,2) - s4*(*this)(0,2)) * inv;

            r(3,0) = (-s3*(*this)(3,3) + s4*(*this)(2,3) - s5*(*this)(1,3)) * inv;
            r(3,1) = ( s3*(*this)(3,2) - s4*(*this)(2,2) + s5*(*this)(1,2)) * inv;
            r(3,2) = (-s0*(*this)(3,3) + s1*(*this)(2,3) - s2*(*this)(1,3)) * inv;
            r(3,3) = ( s0*(*this)(3,2) - s1*(*this)(2,2) + s2*(*this)(1,2)) * inv;
            return r;
        }

        Mat4 inverseTRS() const {
            Vec3<T> c0 = {(*this)(0,0), (*this)(1,0), (*this)(2,0)};  // column 0
            Vec3<T> c1 = {(*this)(0,1), (*this)(1,1), (*this)(2,1)};  // column 1
            Vec3<T> c2 = {(*this)(0,2), (*this)(1,2), (*this)(2,2)};  // column 2
            Vec3<T> t  = {(*this)(0,3), (*this)(1,3), (*this)(2,3)};  // column 3 = translation

            T invS0 = T(1) / dot(c0, c0);
            T invS1 = T(1) / dot(c1, c1);
            T invS2 = T(1) / dot(c2, c2);

            Mat4 r = Mat4::zero();
            r(0,0) = c0.x*invS0;  r(0,1) = c1.x*invS1;  r(0,2) = c2.x*invS2;
            r(1,0) = c0.y*invS0;  r(1,1) = c1.y*invS1;  r(1,2) = c2.y*invS2;
            r(2,0) = c0.z*invS0;  r(2,1) = c1.z*invS1;  r(2,2) = c2.z*invS2;

            r(0,3) = -(r(0,0)*t.x + r(0,1)*t.y + r(0,2)*t.z);
            r(1,3) = -(r(1,0)*t.x + r(1,1)*t.y + r(1,2)*t.z);
            r(2,3) = -(r(2,0)*t.x + r(2,1)*t.y + r(2,2)*t.z);

            r(3,3) = T(1);
            return r;
        }

        //=================================================//

        static Mat4 zero() {
            Mat4 m;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    m.matrix[i][j] = (T)0;
            return m;
        }

        static Mat4<T> identity() { return Mat4(); }

        static Mat4<T> translation(const Vec3<T>& delta) {
            Mat4 m;
            m(0, 3) = delta.x;
            m(1, 3) = delta.y;
            m(2, 3) = delta.z;
            return m;
        }

        static Mat4<T> inverseTranslation(const Vec3<T>& delta) {
            Mat4 m;
            m(0, 3) = -delta.x;
            m(1, 3) = -delta.y;
            m(2, 3) = -delta.z;
            return m;
        }

        static Mat4<T> scale(const Vec3<T>& scale) {
            Mat4 m;
            m(0, 0) = scale.x;
            m(1, 1) = scale.y;
            m(2, 2) = scale.z;
            return m;
        }

        static Mat4<T> inverseScale(const Vec3<T>& scale) {
            Mat4 m;
            m(0, 0) = 1 / scale.x;
            m(1, 1) = 1 / scale.y;
            m(2, 2) = 1 / scale.z;
            return m;
        }

        static Mat4<T> rotationX(T theta) {
            Mat4<T> m;
            T theta_cos = std::cos(theta);
            T theta_sin = std::sin(theta);
            m(1, 1) =  theta_cos;  m(1, 2) = -theta_sin;
            m(2, 1) =  theta_sin;  m(2, 2) =  theta_cos;
            return m;
        }

        static Mat4<T> rotationY(T theta) {
            Mat4<T> m;
            T theta_cos = std::cos(theta);
            T theta_sin = std::sin(theta);
            m(0, 0) =  theta_cos;  m(0, 2) =  theta_sin;
            m(2, 0) = -theta_sin;  m(2, 2) =  theta_cos;
            return m;
        }

        static Mat4<T> rotationZ(T theta) {
            Mat4<T> m;
            T theta_cos = std::cos(theta);
            T theta_sin = std::sin(theta);
            m(0, 0) =  theta_cos;  m(0, 1) = -theta_sin;
            m(1, 0) =  theta_sin;  m(1, 1) =  theta_cos;
            return m;
        }

        static Mat4<T> rotation(T theta, const Vec3<T>& axis) {
            Vec3<T> a = normalize(axis);
            T c = std::cos(theta);
            T s = std::sin(theta);
            T t = T(1) - c;

            Mat4<T> m = Mat4<T>::zero();
            // Column 0
            m(0,0) = t*a.x*a.x + c;
            m(1,0) = t*a.x*a.y + s*a.z;
            m(2,0) = t*a.x*a.z - s*a.y;
            // Column 1
            m(0,1) = t*a.x*a.y - s*a.z;
            m(1,1) = t*a.y*a.y + c;
            m(2,1) = t*a.y*a.z + s*a.x;
            // Column 2
            m(0,2) = t*a.x*a.z + s*a.y;
            m(1,2) = t*a.y*a.z - s*a.x;
            m(2,2) = t*a.z*a.z + c;

            m(3,3) = T(1);
            return m;
        }

        static Mat4 perspective(T fovY, T aspect, T near, T far) {
            T tanHalf = std::tan(fovY * T(0.5));

            Mat4 m = Mat4::zero();
            m(0, 0) =  T(1) / (aspect * tanHalf);
            m(1, 1) =  T(1) / tanHalf;
            m(2, 2) = -(far + near) / (far - near);
            m(2, 3) = -(T(2) * far * near) / (far - near);
            m(3, 2) = -T(1);
            m(3, 3) =  T(0);
            return m;
        }

        // Returns a world to camera lookAt matrix 
        static Mat4 lookAt(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up) {
            Vec3<T> f = normalize(target - eye);
            Vec3<T> r = normalize(cross(f, up));
            Vec3<T> u = cross(r, f);

            Mat4 m = Mat4::zero();
            m(0,0) =  r.x;  m(0,1) =  r.y;  m(0,2) =  r.z;  m(0,3) = -dot(r, eye);
            m(1,0) =  u.x;  m(1,1) =  u.y;  m(1,2) =  u.z;  m(1,3) = -dot(u, eye);
            m(2,0) = -f.x;  m(2,1) = -f.y;  m(2,2) = -f.z;  m(2,3) =  dot(f, eye);
            m(3,3) = T(1);
            return m;
        }

    private:
        T matrix[4][4];
};

using Mat4f = Mat4<float>;
using Mat4d = Mat4<double>;

#endif
