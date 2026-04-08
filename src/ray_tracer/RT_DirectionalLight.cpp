#include "RT_DirectionalLight.h"

RT_DirectionalLight::RT_DirectionalLight(const Vec3d& _direction, const Color& _albedo, double _intensity) {
        type = DIRECTIONALLIGHT;
        direction = _direction;
        albedo = _albedo;
        intensity = _intensity;
    }


Color RT_DirectionalLight::radiate(double distance) const {
    // No falloff — directional light is equally bright everywhere
    return albedo * intensity;
}