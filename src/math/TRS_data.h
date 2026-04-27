#ifndef TRS_DATA_H
#define TRS_DATA_H

#include "math/vec3.h"

/// @brief Simple struct to store Transform components separately.
///        Translation, Rotation (Euler), Scale.
template <typename T>
struct TRSData {
    Vec3<T> translation = {0, 0, 0}; ///< Position
    Vec3<T> scale       = {1, 1, 1}; ///< Scale
    Vec3<T> rotation    = {0, 0, 0}; ///< Rotation in degrees (Euler angles)

    // ---------- incremental updates ----------

    /// @brief Moves the object by delta.
    void translateBy(const Vec3<T>& delta) { translation += delta; }

    /// @brief Adds rotation (degrees).
    void rotateBy(const Vec3<T>& theta) { rotation += theta; }

    /// @brief Multiplies scale.
    void scaleBy(const Vec3<T>& scalar) { scale *= scalar; }

    // ---------- absolute setters ----------

    /// @brief Sets translation.
    void setTranslation(const Vec3<T>& t) { translation = t; }

    /// @brief Sets rotation.
    void setRotation(const Vec3<T>& r) { rotation = r; }

    /// @brief Sets scale.
    void setScale(const Vec3<T>& s) { scale = s; }

    /// @brief Resets to default (identity transform).
    void reset() { *this = TRSData<T>{}; }
};

using TRSDataf = TRSData<float>;
using TRSDatad = TRSData<double>;

#endif