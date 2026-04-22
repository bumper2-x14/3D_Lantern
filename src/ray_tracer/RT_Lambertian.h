#ifndef RT_LAMBERTIAN_H
#define RT_LAMBERTIAN_H

#include "RT_Material.h"
#include "assets/texture.h"

class RT_Lambertian : public RT_Material {
    public:
        RT_Lambertian(const Color& _albedo);
        // This constructor must take a heap allocated texture
        RT_Lambertian(Texture* _tex);
        ~RT_Lambertian();
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, Rayd& scattered_ray) const override;

    private:
        Texture* tex;
        bool own_tex = false;
};

#endif