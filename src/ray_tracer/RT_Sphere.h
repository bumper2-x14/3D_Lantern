#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include "RT_Object.h"
#include "RT_Material.h"
#include "math/TRS_transform.h"

/// @brief Sphere object for ray tracing.
///        Supports transform + intersection.
class RT_Sphere : public RT_Object {
    public:
        /// @brief Creates a sphere with a material.
        RT_Sphere(RT_Material* _material);

        // TODO: Moving sphere constructor

        /// @brief Applies a transform (position, rotation, scale).
        void setTransform(const TRSTransformd& t) override;

        /// @brief Checks intersection between ray and sphere.
        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

        /// @brief Runs basic tests for sphere.
        static void regressionTest();

    private:
        RT_Material* material; ///< Material used for rendering.

};

#endif