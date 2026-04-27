#ifndef RT_DISK_H
#define RT_DISK_H
#include "RT_Object.h"
#include "RT_Material.h"

/// @brief Ray tracing disk (or ring if inner radius > 0).
class RT_Disk : public RT_Object {
public:
    /// @brief Creates a disk or annulus.
    /// @param _center Center of the disk.
    /// @param _normal Surface normal (should be normalized).
    /// @param _radius Outer radius.
    /// @param _inner_radius Inner radius (0 = full disk).
    /// @param _material Material used for shading.
    RT_Disk(const Point3d& _center, const Vec3d& _normal,
            double _radius, double _inner_radius,
            RT_Material* _material);

    /// @brief Creates a disk with default geometry (to be set later).
    RT_Disk(RT_Material* _material);
        
    /// @brief Applies a transform (position, rotation, scale).
    void setTransform(const TRSTransformd& _transform) override;

    /// @brief Checks intersection between ray and disk.
    /// @param ray Incoming ray.
    /// @param t_interval Valid interval for intersection.
    /// @param rec Stores intersection result.
    bool rayIntersect(const Rayd& ray, const Intervald& t_interval,
                      RT_Record& rec) const override;

    /// @brief Runs basic tests for disk intersection.
    static void regressionTest();

private:
    Point3d center; ///< Center of the disk.
    Vec3d   normal; ///< Unit normal of the disk.
    double  radius; ///< Outer radius.
    double  inner_radius; ///< Inner radius (0 = full disk).
    double  D; ///< Plane constant (precomputed).
    RT_Material* material; ///< Material used for rendering.
};

#endif