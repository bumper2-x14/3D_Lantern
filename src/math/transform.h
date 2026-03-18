#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <iostream>
#include <cassert>
#include <chrono>

#include "mat4.h"

class Transform {
    public:
        Mat4f mat, mat_inv;

        //==================================================================//

        Transform(): mat(Mat4f::identity()), mat_inv(Mat4f::identity()) {}

        explicit Transform(const Mat4f& _mat): mat(_mat) {
            mat_inv = mat.inverseTRS();
        }

        Transform(const Mat4f& _mat, const Mat4f& _mat_inv): mat(_mat), mat_inv(_mat_inv) {}

        explicit Transform(const float (&_mat)[4][4]): mat(_mat) {
            mat_inv = mat.inverseTRS();
        }

        Transform inverse() const {
            return Transform(mat_inv, mat);
        }

        Transform operator*(const Transform& t) const {
            return Transform(
                            (mat * t.mat),
                            (t.mat_inv * mat_inv)
                        );
        }

        Transform& operator*=(const Transform& t) {
            mat = mat * t.mat;
            mat_inv = t.mat_inv * mat_inv;
            return *this;
        }

        template <typename T>
        Vec4<T> operator*(const Vec4<T>& v) const {
            return mat * v;
        }
        
        template <typename T>
        Vec3<T> operator*(const Vec3<T>& v) const {
            return mat * v;
        }

        template <typename T>
        Vec3<T> transformNormal(const Vec3<T>& n) const {
            return normaliz1e(mat_inv.transpose() * n);
        }
    

        const Mat4f& getMatrix()        const { return mat;     }
        const Mat4f& getInverseMatrix() const { return mat_inv; }

        //================================================//

        static Transform translate(const Vec3f& delta) {
            return Transform(Mat4f::translation(delta), 
                            Mat4f::inverseTranslation(delta)
                        );
        }

        static Transform scale(const Vec3f& scalar) {
            return Transform(Mat4f::scale(scalar),
                            Mat4f::inverseScale(scalar)
                        );
        }

        static Transform rotateX(float theta) {
            Mat4f m = Mat4f::rotationX(theta);
            return Transform(m, m.transpose());
        }

        static Transform rotateY(float theta) {
            Mat4f m = Mat4f::rotationY(theta);
            return Transform(m, m.transpose());
        }

        static Transform rotateZ(float theta) {
            Mat4f m = Mat4f::rotationZ(theta);
            return Transform(m, m.transpose());
        }

        static Transform rotate(float theta, const Vec3f& axis){
            Mat4f m = Mat4f::rotation(theta, axis);
            return Transform(m, m.transpose());
        }

        // Transform::lookAt
        // mat     = cameraToWorld — used in raytracing to transform rays from camera space into world space
        // mat_inv = worldToCamera — used as the view matrix in OpenGL 
        static Transform lookAt(const Vec3f& eye, const Vec3f& target, const Vec3f& up) {
            Vec3f f = normalize(target - eye);
            Vec3f r = normalize(cross(f, up));
            Vec3f u = cross(r, f);

            Mat4f camToWorld = Mat4f::zero();
            camToWorld(0,0) =  r.x;  camToWorld(1,0) =  r.y;  camToWorld(2,0) =  r.z;
            camToWorld(0,1) =  u.x;  camToWorld(1,1) =  u.y;  camToWorld(2,1) =  u.z;
            camToWorld(0,2) = -f.x;  camToWorld(1,2) = -f.y;  camToWorld(2,2) = -f.z;
            camToWorld(0,3) = eye.x; camToWorld(1,3) = eye.y; camToWorld(2,3) = eye.z;
            camToWorld(3,3) = 1.0;

            // worldToCamera: transpose R block, recompute translation (no full inverse needed)
            Mat4f worldToCam = Mat4f::zero();
            worldToCam(0,0) =  r.x;  worldToCam(0,1) =  r.y;  worldToCam(0,2) =  r.z;
            worldToCam(1,0) =  u.x;  worldToCam(1,1) =  u.y;  worldToCam(1,2) =  u.z;
            worldToCam(2,0) = -f.x;  worldToCam(2,1) = -f.y;  worldToCam(2,2) = -f.z;
            worldToCam(0,3) = -dot(r, eye);
            worldToCam(1,3) = -dot(u, eye);
            worldToCam(2,3) =  dot(f, eye);
            worldToCam(3,3) = 1.0;

            return Transform(camToWorld, worldToCam);
        }
};


inline Transform inverse(const Transform& t) {
    return t.inverse();
}


#endif