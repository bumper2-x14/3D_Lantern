#ifndef RT_LAMBERTIAN_H
#define RT_LAMBERTIAN_H

#include "RT_Material.h"
#include "assets/texture.h"

/// @brief Lambertian material (diffuse surface).
///        Scatters light uniformly in all directions.
class RT_Lambertian : public RT_Material {
    public:
        /// @brief Creates a diffuse material with constant color.
        /// @param _albedo Base color.
        RT_Lambertian(const Color& _albedo);

        /// @brief Creates a diffuse material using a texture.
        /// @param _tex Texture (must be allocated on heap).
        // This constructor must take a heap allocated texture
        RT_Lambertian(Texture* _tex);

        /// @brief Destructor (deletes texture if owned).
        ~RT_Lambertian();

        /// @brief Computes scattered ray for diffuse reflection.
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const override;

    private:
        Texture* tex; ///< Texture controlling surface color.
        bool own_tex = false; ///< True if this class owns the texture.
};

#endif