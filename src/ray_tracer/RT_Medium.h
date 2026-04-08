#ifndef RT_MEDIUM_H
#define RT_MEDIUM_H

#include "RT_Object.h"
#include "RT_Material.h"
#include "assets/texture.h"
#include "RT_Record.h"

class RT_Medium : public RT_Object {
    public:
        RT_Medium(RT_Object* _bounding_obj, double _density, const Color& _albedo);
        RT_Medium(RT_Object* _bounding_obj, double _density, Texture* _tex);
        ~RT_Medium();

        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

    private:
        RT_Object* bounding_obj;
        RT_Material* phase_func;
        double density_neg_inv;
};

#endif