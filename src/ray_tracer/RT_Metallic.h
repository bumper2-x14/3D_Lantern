#ifndef RT_METALLIC_H
#define RT_METALLIC_H

#include "RT_Material.h"
#include "RT_Record.h"

class RT_Metallic : public RT_Material {
    public:
        RT_Metallic(const Color& _albedo, double _fuzz);
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const override;

    private:
        Color albedo;
        double fuzz;
};

#endif