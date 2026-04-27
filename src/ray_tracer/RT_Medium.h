#ifndef RT_MEDIUM_H
#define RT_MEDIUM_H

#include "RT_Object.h"
#include "RT_Material.h"
#include "assets/texture.h"
#include "RT_Record.h"

/// @brief Volumetric medium (fog/smoke) around a bounding object.
///        Uses a phase function (isotropic) to scatter rays inside.
class RT_Medium : public RT_Object {
    public:
        /// @brief Creates a medium with constant color.
        /// @param _bounding_obj Shape that defines the volume.
        /// @param _density Density of the medium.
        /// @param _albedo Color of the medium.
        RT_Medium(RT_Object* _bounding_obj, double _density, const Color& _albedo);

        /// @brief Creates a medium using a texture.
        /// @param _bounding_obj Shape that defines the volume.
        /// @param _density Density of the medium.
        /// @param _tex Texture used for color/scattering.
        RT_Medium(RT_Object* _bounding_obj, double _density, Texture* _tex);

        /// @brief Destructor (cleans allocated data if needed).
        ~RT_Medium();

        /// @brief Checks ray interaction with the medium.
        ///        Computes scattering inside the volume.
        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

    private:
        RT_Object* bounding_obj; ///< Shape defining the volume limits.
        RT_Material* phase_func; ///< Phase function (usually isotropic).
        double density_neg_inv; ///< Precomputed value for sampling distance.
};

#endif