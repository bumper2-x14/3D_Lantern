#ifndef RT_ISOTROPIC_H
#define RT_ISOTROPIC_H

#include "RT_Material.h"
#include "assets/texture.h"

class RT_Isotropic : public RT_Material {
    public:
        RT_Isotropic(const Color& _albedo);
        RT_Isotropic(Texture* _tex);
        ~RT_Isotropic();
        bool rayScatter(const Rayd& ray_in, const RT_Record& rec, Color& attenuation, 
                                                        Rayd& scattered_ray) const override;
    private:
        Texture* tex;   
};

#endif