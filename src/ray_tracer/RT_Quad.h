#ifndef RT_QUAD_H
#define RT_QUAD_H
#include "RT_Object.h"
#include "RT_Material.h"

/// @brief Quad (rectangle) for ray tracing.
///        Defined by a corner point and two edge vectors.
class RT_Quad : public RT_Object {
public:
    /// @brief Default constructor.
    RT_Quad();

    /// @brief Creates a quad from corner and edge vectors.
    /// @param _Q Corner point.
    /// @param _u First edge vector.
    /// @param _v Second edge vector.
    /// @param _material Material used for shading.
    // Q = corner point, u/v = edge vectors
    RT_Quad(const Point3d& _Q, const Vec3d& _u, const Vec3d& _v, RT_Material* _material);

    /// @brief Creates a quad with default geometry (to be transformed later).
    RT_Quad(RT_Material* _material);

    /// @brief Applies a transform (position, rotation, scale).
    void setTransform(const TRSTransformd& _transform) override;

    /// @brief Checks intersection between ray and quad.
    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

    /// @brief Runs basic tests for quad.
    static void regressionTest();

private:
    Point3d Q0 = Point3d(0, 0, 0);  ///< Default corner.
    Vec3d   u0 = Vec3d(1, 0, 0);    ///< Default u vector.
    Vec3d   v0 = Vec3d(0, 1, 0);    ///< Default v vector.
    
    Point3d Q; ///< Transformed corner.
    Vec3d   u, v; ///< Transformed edge vectors.
    Vec3d   normal;   ///< Precomputed normal (cross(u, v)).
    Vec3d   w;        ///< Helper vector for intersection.
    double  D;        ///< Plane constant.
    RT_Material* material; ///< Material used for rendering.
};
#endif