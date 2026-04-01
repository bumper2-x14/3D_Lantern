#include "RT_Medium.h"
#include "RT_Isotropic.h"

RT_Medium::RT_Medium(RT_Object* _bounding_obj, double _density, const Color& _albedo) : 
                    bounding_obj(_bounding_obj), density_neg_inv(-1.0 / _density), phase_func(new RT_Isotropic(_albedo)) {
        setBoundingBox(bounding_obj->getBoundingBox());
}

RT_Medium::RT_Medium(RT_Object* _bounding_obj, double _density, Texture* _tex) :
    bounding_obj(_bounding_obj), density_neg_inv(-1.0 / _density), phase_func(new RT_Isotropic(_tex)) {
        setBoundingBox(bounding_obj->getBoundingBox());
}

RT_Medium::~RT_Medium() {
    delete phase_func;
}

bool RT_Medium::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    RT_Record rc1, rc2;
    
    // rc1/rc2 t values are in local space — compute the scale factor
    // by transforming the ray direction through the boundary's inverse
    Mat4d inv = bounding_obj->getInverse();
    double local_dir_len = (inv * ray.getDirection()).length();

    double offset = 0.001 * local_dir_len;
    if (!bounding_obj->rayIntersect(ray, Intervald::universe, rc1)) return false;
    if (!bounding_obj->rayIntersect(ray, Intervald(rc1.t + offset, infinity<double>), rc2)) return false;

    double world_thickness = (rc2.t - rc1.t) * local_dir_len;

    double t_near = std::max(rc1.t, t_interval.min);
    double t_far = std::min(rc2.t, t_interval.max);
    if (t_near >= t_far) return false;
    t_near = std::max(t_near, 0.0);

    double scatt_dist = density_neg_inv * std::log(randomizer<double>(1e-9, 1.0));

    if (scatt_dist > world_thickness) return false;

    rec.t = t_near + scatt_dist * local_dir_len;
    rec.p = ray.at(rec.t);
    rec.normal = Vec3d(1, 0, 0);
    rec.front_face = true;
    rec.material = phase_func;

    return true;
}