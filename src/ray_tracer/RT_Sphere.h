#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include "RT_Object.h"
#include "RT_Material.h"

class RT_Sphere : public RT_Object {
    public:
        RT_Sphere(const Point3d& _center, double _radius, RT_Material* _material);
        // TODO: Moving sphere constructor
        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

        // TODO
        // getSphereUV(double u, double v) ...

        static void regressionTest();

    private:
        Point3d center;
        double radius;
        RT_Material* material;
};

#endif