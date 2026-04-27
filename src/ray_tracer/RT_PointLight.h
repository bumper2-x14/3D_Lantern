#ifndef RT_POINTLIGHT_H
#define RT_POINTLIGHT_H

#include "RT_Light.h"
#include "math/point3.h"
#include "math/color.h"

/// @brief Point light (emits light from a position in space).
class RT_PointLight : public RT_Light {
    public:
        /// @brief Creates a point light.
        /// @param _position Light position.
        /// @param _albedo Light color.
        /// @param _intensity Light strength.
        RT_PointLight(const Point3d& _position, const Color& _albedo, double _intensity = 1.0);

        /// @brief Returns light contribution based on distance.
        Color radiate(double distance) const override;
};

#endif