#include "RT_Metallic.h"


RT_Metallic::RT_Metallic(const Color& _albedo, double _fuzz) : albedo(_albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

bool RT_Metallic::rayScatter(const Rayd& ray_in, const RT_Record& rec, 
                                Color& attenuation, Rayd& scattered_ray) const {

    Vec3d reflection = reflect(normalize(ray_in.getDirection()), rec.normal);
    // fuzz > 0 adds blur to the reflection — 0 = perfect mirror
    reflection = reflection + fuzz * randomUnitVector<double>();
    scattered_ray = Rayd(rec.p, reflection);
    attenuation = albedo;
    return dot(scattered_ray.getDirection(), rec.normal) > 0;
}
