#ifndef RT_CYLINDER_H
#define RT_CYLINDER_H
#include "RT_Object.h"
#include "RT_Material.h"

/// @brief Ray tracing cylinder object.
///        Handles intersection tests and transforms.
class RT_Cylinder : public RT_Object {
public:
    /// @brief Creates a cylinder.
    /// @param _capped True if top and bottom disks are included.
    /// @param _material Material used for shading.
    RT_Cylinder(bool _capped, RT_Material* _material);

    /// @brief Applies a transform (position, rotation, scale).
    void setTransform(const TRSTransformd& t) override;

    /// @brief Checks intersection between ray and cylinder.
    /// @param ray Incoming ray.
    /// @param t_interval Valid interval for intersection.
    /// @param rec Stores intersection result.
    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;


private:
    bool capped; ///< True if cylinder has top and bottom caps.
    RT_Material* material; ///< Material used for rendering.
};
#endif