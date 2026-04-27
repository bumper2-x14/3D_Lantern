#ifndef RT_DIELECTRIC_H
#define RT_DIELECTRIC_H

#include "RT_Material.h"

/// @brief Dielectric material (glass, water, etc).
///        Handles refraction and reflection.
class RT_Dielectric : public RT_Material {
    public:
        /// @brief Creates a dielectric material.
        /// @param _refraction_index Index of refraction (e.g. glass ~1.5).
        RT_Dielectric(double _refraction_index);

        /// @brief Computes how the ray is scattered (reflection/refraction).
        /// @param ray_in Incoming ray.
        /// @param rec Hit record.
        /// @param attenuation Output color attenuation.
        /// @param scattered_ray Output scattered ray.
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const override;

    private:
        double refraction_index; ///< Index of refraction.
};

#endif