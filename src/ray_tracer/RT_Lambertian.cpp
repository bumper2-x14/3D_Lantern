#include "RT_Lambertian.h"
#include "RT_Record.h"

RT_Lambertian::RT_Lambertian(const Color& _albedo) : albedo(_albedo) {}

bool RT_Lambertian::rayScatter(const Rayd& ray_in, const RT_Record& rec, 
                                    Color& attenuation, Rayd& scattered_ray) const {
    Vec3d scatter_dir = rec.normal + randomUnitVector<double>();   
    
    if (scatter_dir.nearZero()) {
        scatter_dir = rec.normal;
    }

    scattered_ray = Rayd(rec.p, scatter_dir);
    attenuation = albedo;
    return true;
}