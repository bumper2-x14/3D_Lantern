#ifndef RT_METALLIC_H
#define RT_METALLIC_H

#include "RT_Material.h"
#include "RT_Record.h"

/// @brief Metallic material (reflective surface with optional roughness).
class RT_Metallic : public RT_Material {
    public:
        /// @brief Creates a metallic material.
        /// @param _albedo Base color.
        /// @param _fuzz Roughness (0 = perfect mirror, higher = blurrier).
        RT_Metallic(const Color& _albedo, double _fuzz);

        /// @brief Computes reflected/scattered ray.
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const override;

    private:
        Color albedo; ///< Surface color.
        double fuzz; ///< Reflection roughness.
};

#endif