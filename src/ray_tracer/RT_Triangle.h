#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include "RT_Object.h"
#include "RT_Material.h"

/// @brief Triangle primitive for ray tracing.
///        Supports per-vertex normals and UVs.
class RT_Triangle : public RT_Object {
public:
    /// @brief Creates a triangle with vertices, normals and UVs.
    RT_Triangle(const Vec3d& v0, const Vec3d& v1, const Vec3d& v2,
                const Vec3d& n0, const Vec3d& n1, const Vec3d& n2,
                const Vec2d& uv0, const Vec2d& uv1, const Vec2d& uv2,
                RT_Material* material);

    /// @brief Applies a transform (position, rotation, scale).
    void setTransform(const TRSTransformd& t) override;

    /// @brief Checks intersection between ray and triangle.
    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

private:
    Vec3d v0, v1, v2; ///< Triangle vertices.
    Vec3d n0, n1, n2; ///< Per-vertex normals.
    Vec2d uv0, uv1, uv2; ///< Texture coordinates.
    RT_Material* material; ///< Material used for rendering.

    /// @brief Computes bounding box in local space.
    BoundingBoxd computeLocalBBox() const;
};
#endif