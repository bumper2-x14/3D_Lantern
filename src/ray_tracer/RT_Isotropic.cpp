#include "RT_Isotropic.h"
#include "assets/color_texture.h"
#include "RT_Record.h"

RT_Isotropic::RT_Isotropic(const Color& _albedo) : tex(new ColorTexture(_albedo)) {}

RT_Isotropic::RT_Isotropic(Texture* _tex) : tex(_tex) {}

RT_Isotropic::~RT_Isotropic() {
    delete tex;
}

bool RT_Isotropic::rayScatter(const Rayd& ray_in, const RT_Record& rec, 
                    Color& attenuation, Rayd& scattered_ray) const {
    scattered_ray = Rayd(rec.p, randomUnitVector<double>(), ray_in.getTime());
    attenuation = tex->sample(rec.uv, rec.p);
    return true;
}