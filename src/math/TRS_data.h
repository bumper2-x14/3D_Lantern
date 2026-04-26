#ifndef TRS_DATA_H
#define TRS_DATA_H

#include "math/vec3.h"

template <typename T>
struct TRSData {
    Vec3<T> translation = {0, 0, 0};
    Vec3<T> scale       = {1, 1, 1};
    Vec3<T> rotation    = {0, 0, 0};  // euler angles per axis, degrees

    // Incremental mutators
    void translateBy(const Vec3<T>& delta) { translation += delta; }
    void rotateBy(const Vec3<T>& theta) { rotation += theta; }
    void scaleBy(const Vec3<T>& scalar) { scale *= scalar; }

    // Absolute mutators
    void setTranslation(const Vec3<T>& t) { translation = t; }
    void setRotation(const Vec3<T>& r) { rotation = r; }
    void setScale(const Vec3<T>& s) { scale = s; }

    void reset() { *this = TRSData<T>{}; }
};

using TRSDataf = TRSData<float>;
using TRSDatad = TRSData<double>;

#endif