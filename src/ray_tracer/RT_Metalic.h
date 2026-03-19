#ifndef RT_METALIC_H
#define RT_METALIC_H

#include "RT_Material.h"
#include "RT_Record.h"

class RT_Metalic : public RT_Material {
    public:
        RT_Metalic(const Color& _albedo, double _fuzz);
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const override;

    private:
        Color albedo;
        double fuzz;
};

#endif