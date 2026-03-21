#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include "math/ray.h"
#include "math/color.h"

struct RT_Record;

class RT_Material {
    public:
        virtual ~RT_Material() = default;

        virtual bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const {
            return false;
        }
};

#endif