#ifndef RT_CONE_H
#define RT_CONE_H

#include "RT_Object.h"

class RT_Cone : public RT_Object {
    public:
        RT_Cone(const Point3d& _apex, double _half_angle, double _y_min, double _y_max,
                    bool _capped, RT_Material* _material);

        bool rayIntersect(const Rayd& ray, const Intervald& t_interval,
                          RT_Record& rec) const override;

        static void regressionTest();

private:
        Point3d apex;
        double  half_angle;       // radians — tan(half_angle) = radius/height
        double  y_min, y_max;     // truncation planes
        bool    capped;
        RT_Material* material;
};

#endif