#include "RT_Record.h"
#include "RT_Dielectric.h"


RT_Dielectric::RT_Dielectric(double _refraction_index) : refraction_index(_refraction_index) {}
    
bool RT_Dielectric::rayScatter(const Rayd& ray_in, const RT_Record& rec, 
                                    Color& attenuation, Rayd& scattered_ray) const {
    // No color absortation for glass
    attenuation = Color(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;
    Vec3d unit_dir = normalize(ray_in.getDirection());
    double cos_theta = std::min(dot(-unit_dir, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0; // total internal reflection
    Vec3d direction;

    if (cannot_refract || schlick(cos_theta, ri) > randomizer<double>(0, 1))
        direction = reflect(unit_dir, rec.normal);
    else
        direction = refract(unit_dir, rec.normal, ri);

    scattered_ray = Rayd(rec.p, direction);
    return true;

}