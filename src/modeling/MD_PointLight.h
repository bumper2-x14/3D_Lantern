#ifndef MD_POINTLIGHT_H
#define MD_POINTLIGHT_H

#include "math/vec3.h"
#include "math/color.h"

class MD_PointLight {
public:
    MD_PointLight()
        : position(0.f, 3.f, 0.f)
        , color(1.f, 1.f, 1.f)
        , intensity(1.f)
    {}

    MD_PointLight(const Vec3f& _pos, const Color& _color, float _intensity)
        : position(_pos)
        , color(_color)
        , intensity(_intensity)
    {}

    // ── Getters ───────────────────────────────────────
    const Vec3f& getPosition() const { return position; }
    const Color& getColor() const { return color; }
    float getIntensity() const { return intensity;  }

    // ── Setters ───────────────────────────────────────
    void setPosition (const Vec3f& p) { position  = p; }
    void setColor (const Vec3f& c) { color     = c; }
    void setIntensity(float i) { intensity = i; }

private:
    Vec3f position;
    Color color;
    float intensity;
};

#endif