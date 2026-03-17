#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include "RT_Object.h"

class RT_Sphere : public RT_Object {
    public:
        RT_Sphere(const Point3d& _center, double _radius);
        // TODO: Moving sphere constructor
        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

        // TODO
        // getSphereUV(double u, double v) ...

    private:
        Point3d center;
        double radius;
};

#endif