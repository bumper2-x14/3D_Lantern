#ifndef RT_ISOTROPIC_H
#define RT_ISOTROPIC_H

#include "RT_Material.h"
#include "assets/texture.h"

/// @brief Isotropic material (used for volumes like fog/smoke).
///        Scatters rays in random directions.
class RT_Isotropic : public RT_Material {
    public:
        /// @brief Creates isotropic material with constant color.
        /// @param _albedo Base color.
        RT_Isotropic(const Color& _albedo);

        /// @brief Creates isotropic material using a texture.
        /// @param _tex Texture used for scattering color.
        RT_Isotropic(Texture* _tex);

        /// @brief Destructor (cleans texture if needed).
        ~RT_Isotropic();

        /// @brief Computes random scattering inside the medium.
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, 
                                                        Rayd& scattered_ray) const override;
    private:
        Texture* tex; ///< Texture controlling color of the medium.
};

#endif