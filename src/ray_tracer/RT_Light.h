#ifndef RT_LIGHT_H
#define RT_LIGHT_H

#include "math/color.h"
#include "math/vec3.h"
#include "math/point3.h"

enum RT_LightType {
    POINTLIGHT,
    DIRECTIONALLIGHT
};

class RT_Light {
    public:
        RT_LightType type;
        Point3d position = Point3d(0.0, 0.0, 0.0);
        Vec3d direction = Vec3d(0.0, 0.0, 0.0);
        Color albedo;
        double intensity; // 1.0 is neutral
        
        virtual ~RT_Light() = default;

        virtual Color radiate(double distance = 0.0) const = 0;
};

#endif