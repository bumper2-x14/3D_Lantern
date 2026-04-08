#ifndef RT_POINTLIGHT_H
#define RT_POINTLIGHT_H

#include "RT_Light.h"
#include "math/point3.h"
#include "math/color.h"

class RT_PointLight : public RT_Light {
    public:
        RT_PointLight(const Point3d& _position, const Color& _albedo, double _intensity = 1.0);

        Color radiate(double distance) const override;
};

#endif