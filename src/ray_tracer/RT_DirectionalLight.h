#ifndef RT_DIRECTIONALLIGHT_H
#define RT_DIRECTIONALLIGHT_H

#include "RT_Light.h"

class RT_DirectionalLight : public RT_Light {
    public:
        RT_DirectionalLight(const Vec3d& _direction, const Color& _albedo, double _intensity);

        // distance is no needed for directional lights
        Color radiate(double distance = 0.0) const override;
};


#endif