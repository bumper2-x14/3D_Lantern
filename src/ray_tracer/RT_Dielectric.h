#ifndef RT_DIELECRIC_H
#define RT_DIELECRIC_H

#include "RT_Material.h"

class RT_Dielecric : public RT_Material {
    public:
        RT_Dielecric(double _refraction_index);
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const override;

    private:
        double refraction_index;
};

#endif
