#include "RT_PointLight.h"

RT_PointLight::RT_PointLight(const Point3d& _position, const Color& _albedo, double _intensity) {
        type = POINTLIGHT;
        position = _position;
        albedo = _albedo;
        intensity = _intensity;
}


Color RT_PointLight::radiate(double distance = 0.0) const {
    return albedo * intensity / distance * distance;
}