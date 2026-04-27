#ifndef RT_LIGHT_H
#define RT_LIGHT_H

#include "math/color.h"
#include "math/vec3.h"
#include "math/point3.h"

/// @brief Types of lights supported by the ray tracer.
enum RT_LightType {
    POINTLIGHT,
    DIRECTIONALLIGHT
};

/// @brief Base class for all lights in the ray tracer.
class RT_Light {
    public:
        RT_LightType type; ///< Type of the light.

        Point3d position = Point3d(0.0, 0.0, 0.0); ///< Position (used for point lights).
        Vec3d direction = Vec3d(0.0, 0.0, 0.0); ///< Direction (used for directional lights).

        Color albedo; ///< Light color.
        double intensity; ///< Light strength (1.0 = default).

        /// @brief Virtual destructor.
        virtual ~RT_Light() = default;

        /// @brief Returns the light contribution.
        /// @param distance Distance to the light (used for attenuation if needed).
        virtual Color radiate(double distance = 0.0) const = 0;
};

#endif