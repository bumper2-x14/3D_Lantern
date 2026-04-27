#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "math/ray.h"
#include "math/color.h"

struct RT_Record;

/// @brief Base class for all ray tracing materials.
///        Defines how rays interact with surfaces.
class RT_Material {
    public:
        /// @brief Virtual destructor.
        virtual ~RT_Material() = default;

        /// @brief Computes how a ray scatters after hitting a surface.
        /// @param ray_in Incoming ray.
        /// @param rec Hit information.
        /// @param attenuation Output color attenuation.
        /// @param scattered_ray Output scattered ray.
        /// @return true if the ray is scattered, false otherwise.
        virtual bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const {
            return false;
        }
};

#endif