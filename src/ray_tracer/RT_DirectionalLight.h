#ifndef RT_DIRECTIONALLIGHT_H
#define RT_DIRECTIONALLIGHT_H

#include "RT_Light.h"

/// @brief Directional light (like sun), same direction everywhere.
class RT_DirectionalLight : public RT_Light {
    public:
        /// @brief Creates a directional light.
        /// @param _direction Direction of the light.
        /// @param _albedo Light color.
        /// @param _intensity Light strength.
        RT_DirectionalLight(const Vec3d& _direction, const Color& _albedo, double _intensity);

        /// @brief Returns light color (distance is ignored for directional lights).
        Color radiate(double distance = 0.0) const override;
};

#endif