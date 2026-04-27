#ifndef TRS_TRANSFORM_H
#define TRS_TRANSFORM_H

#include "TRS_data.h"
#include "mat4.h"

/// @brief Builds and stores a TRS transform matrix from translation, rotation and scale.
///        Recomputes the matrix only when something changes.
template <typename T> class TRSTransform {
    public:
        TRSData<T> trs; ///< Raw translation, rotation and scale values.

        /// @brief Default constructor.
        TRSTransform() = default;

        // Incremental mutators

        /// @brief Adds a translation.
        void translate(const Vec3<T>& delta)  { trs.translation += delta; changed = true; }

        /// @brief Adds a rotation.
        void rotate   (const Vec3<T>& delta)  { trs.rotation += delta; changed = true; }

        /// @brief Multiplies the current scale.
        void scaleBy  (const Vec3<T>& delta)  { trs.scale *= delta; changed = true; }

        // Absolute mutators

        /// @brief Sets translation directly.
        void setTranslation(const Vec3<T>& t) { trs.translation = t;     changed = true; }

        /// @brief Sets rotation directly.
        void setRotation   (const Vec3<T>& r) { trs.rotation = r;     changed = true; }

        /// @brief Sets scale directly.
        void setScale      (const Vec3<T>& s) { trs.scale = s;     changed = true; }

        // Reset

        /// @brief Resets transform to identity values.
        void reset() { trs = TRSData<T>{}; changed = true; }

        /// @brief Returns transform matrix, recomputing it if needed.
        const Mat4<T>& getMatrix() const { recompute(); return mat; }

        /// @brief Returns inverse transform matrix, recomputing it if needed.
        const Mat4<T>& getInverse() const { recompute(); return mat_inv; }

        // Raytracer handoff — called once at render time 

        /// @brief Builds the inverse matrix directly for ray tracing.
        Mat4<T> buildInverse() const {
            Mat4<T> iS  = Mat4<T>::inverseScale(trs.scale);
            Mat4<T> iRx = Mat4<T>::rotationX(-trs.rotation.x);
            Mat4<T> iRy = Mat4<T>::rotationY(-trs.rotation.y);
            Mat4<T> iRz = Mat4<T>::rotationZ(-trs.rotation.z);
            Mat4<T> iT  = Mat4<T>::inverseTranslation(trs.translation);
            return iS * iRx * iRy * iRz * iT;
        }

    private:
        mutable Mat4<T> mat; ///< Cached transform matrix.
        mutable Mat4<T> mat_inv; ///< Cached inverse matrix.
        mutable bool changed = true; ///< True when matrices need update.

        /// @brief Converts degrees to radians.
        static constexpr T deg2rad(T d) { return d * T(M_PI) / T(180); }

        /// @brief Recomputes matrices only if transform values changed.
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

#endif