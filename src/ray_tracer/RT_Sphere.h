#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include "RT_Object.h"
#include "RT_Material.h"
#include "math/TRS_transform.h"

class RT_Sphere : public RT_Object {
    public:
        RT_Sphere(RT_Material* _material);
        // TODO: Moving sphere constructor
        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;


        static void regressionTest();

    private:
        RT_Material* material;

};

#endif