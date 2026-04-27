#ifndef RT_CONE_H
#define RT_CONE_H

#include "RT_Object.h"

/// @brief Ray tracing cone object.
///        Handles intersection and transform for a cone shape.
class RT_Cone : public RT_Object {
    public:
        /// @brief Creates a cone.
        /// @param _capped True if the cone has a base cap.
        /// @param _material Material used for shading.
        RT_Cone(bool _capped, RT_Material* _material);

        /// @brief Applies a transform (position, rotation, scale).
        void setTransform(const TRSTransformd& t) override;

        /// @brief Checks intersection between ray and cone.
        /// @param ray Incoming ray.
        /// @param t_interval Valid interval for intersection.
        /// @param rec Stores intersection result.
        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

        /// @brief Runs basic tests for cone intersection.
        static void regressionTest();

private:
        bool    capped; ///< True if cone has a base.
        RT_Material* material; ///< Material used for rendering.
};

#endif