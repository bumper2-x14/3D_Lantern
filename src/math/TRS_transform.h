#ifndef TRS_TRANSFORM_H
#define TRS_TRANSFORM_H

#include "mat4.h"


template <typename T> struct TRSData {
    Vec3<T> translation = {0, 0, 0};
    Vec3<T> scale       = {1, 1, 1};
    Vec3<T> rotation    = {0, 0, 0};  // euler angles per axis, degrees
};


template <typename T> class TRSTransform {
    public:
        TRSData<T> trs;

        TRSTransform() = default;

        // Incremental mutators
        void translate(const Vec3<T>& delta)  { trs.translation += delta; changed = true; }
        void rotate   (const Vec3<T>& delta)  { trs.rotation += delta; changed = true; }
        void scaleBy  (const Vec3<T>& delta)  { trs.scale *= delta; changed = true; }

        // Absolute mutators
        void setTranslation(const Vec3<T>& t) { trs.translation = t;     changed = true; }
        void setRotation   (const Vec3<T>& r) { trs.rotation = r;     changed = true; }
        void setScale      (const Vec3<T>& s) { trs.scale = s;     changed = true; }

        // Reset
        void reset() { trs = TRSData<T>{}; changed = true; }

        const Mat4<T>& getMatrix() const { recompute(); return mat; }
        const Mat4<T>& getInverse() const { recompute(); return mat_inv; }

        // Raytracer handoff — called once at render time 
        Mat4<T> buildInverse() const {
            Mat4<T> iS  = Mat4<T>::inverseScale(trs.scale);
            Mat4<T> iRx = Mat4<T>::rotationX(-trs.rotation.x);
            Mat4<T> iRy = Mat4<T>::rotationY(-trs.rotation.y);
            Mat4<T> iRz = Mat4<T>::rotationZ(-trs.rotation.z);
            Mat4<T> iT  = Mat4<T>::inverseTranslation(trs.translation);
            return iS * iRx * iRy * iRz * iT;
        }

    private:
        mutable Mat4<T> mat;
        mutable Mat4<T> mat_inv;
        mutable bool changed = true;

        static constexpr T deg2rad(T d) { return d * T(M_PI) / T(180); }

        void recompute() const {
            if (!changed) return;
            mat = Mat4<T>::translation(trs.translation)
                * Mat4<T>::rotationX(deg2rad(trs.rotation.x))
                * Mat4<T>::rotationY(deg2rad(trs.rotation.y))
                * Mat4<T>::rotationZ(deg2rad(trs.rotation.z))
                * Mat4<T>::scale(trs.scale);

            mat_inv = Mat4<T>::inverseScale(trs.scale)
                    * Mat4<T>::rotationX(deg2rad(-trs.rotation.x))
                    * Mat4<T>::rotationY(deg2rad(-trs.rotation.y))
                    * Mat4<T>::rotationZ(deg2rad(-trs.rotation.z))
                    * Mat4<T>::inverseTranslation(trs.translation);

            changed = false;
        }

};

using TRSTransformf = TRSTransform<float>; 
using TRSTransformd = TRSTransform<double>;
using TRSDataf = TRSData<float>;
using TRSDatad = TRSData<double>;

#endif