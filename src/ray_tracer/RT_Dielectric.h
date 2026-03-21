#ifndef RT_DIELECTRIC_H
#define RT_DIELECTRIC_H

#include "RT_Material.h"

class RT_Dielectric : public RT_Material {
    public:
        RT_Dielectric(double _refraction_index);
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const override;

    private:
        double refraction_index;
};

#endif
