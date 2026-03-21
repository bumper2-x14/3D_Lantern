#ifndef RT_LAMBERTIAN_H
#define RT_LAMBERTIAN_H

#include "RT_Material.h"

class RT_Lambertian : public RT_Material {
    public:
        RT_Lambertian(const Color& _albedo);
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const override;

    private:
        Color albedo;
};

#endif