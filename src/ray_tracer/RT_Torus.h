#ifndef RT_TORUS_H
#define RT_TORUS_H

#include "RT_Object.h"
#include "RT_Material.h"
#include "math/TRS_transform.h"

// Torus in local space: fixed ratio R=1, r=0.25.
// Resize and reposition entirely via the TRS transform.
// The hole axis is the +Y axis.

/// @brief Torus object for ray tracing.
///        Uses implicit equation and quartic solver.
class RT_Torus : public RT_Object {
public:
    /// @brief Creates a torus with a material.
    RT_Torus(RT_Material* material);

    /// @brief Applies a transform (position, rotation, scale).
    void setTransform(const TRSTransformd& t) override;

    /// @brief Checks intersection between ray and torus.
    bool rayIntersect(const Rayd& ray, const Intervald& t_interval,
                      RT_Record& rec) const override;

    /// @brief Runs basic tests for torus.
    static void regressionTest();

private:
    static constexpr double R = 1.0;    ///< Major radius (fixed).
    static constexpr double r = 0.25;   ///< Minor radius (fixed).
    RT_Material* material; ///< Material used for rendering.

    /// @brief Solves quartic equation:
    ///        c4*t^4 + c3*t^3 + c2*t^2 + c1*t + c0 = 0
    /// @return Number of real roots (stored in out[]).
    static int solveQuartic(double c4, double c3, double c2,
                            double c1, double c0,
                            double out[4]);
};

#endif