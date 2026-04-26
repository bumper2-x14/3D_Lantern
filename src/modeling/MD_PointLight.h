#ifndef MD_POINTLIGHT_H
#define MD_POINTLIGHT_H

#include "math/vec3.h"
#include "math/color.h"

/// @brief Simple point light (emits light in all directions from a position).
class MD_PointLight {
public:
    /// @brief Default light (placed a bit above origin, white color, medium intensity).
    MD_PointLight()
        : position(0.f, 3.f, 0.f)
        , color(1.f, 1.f, 1.f)
        , intensity(1.f)
    {}

    /// @brief Creates a point light with custom values.
    /// @param _pos Light position.
    /// @param _color Light color.
    /// @param _intensity Light strength.
    MD_PointLight(const Vec3f& _pos, const Color& _color, float _intensity)
        : position(_pos)
        , color(_color)
        , intensity(_intensity)
    {}

    // ── Getters ───────────────────────────────────────

    /// @brief Returns the light position.
    const Vec3f& getPosition() const { return position; }

    /// @brief Returns the light color.
    const Color& getColor() const { return color; }

    /// @brief Returns the light intensity.
    float getIntensity() const { return intensity;  }

    // ── Setters ───────────────────────────────────────

    /// @brief Updates the light position.
    void setPosition (const Vec3f& p) { position  = p; }

    /// @brief Updates the light color.
    void setColor (const Vec3f& c) { color     = c; }

    /// @brief Updates the light intensity.
    void setIntensity(float i) { intensity = i; }

private:
    Vec3f position; ///< Position of the light in the scene.
    Color color; ///< Color of the light.
    float intensity; ///< Strength of the light.
};

#endif