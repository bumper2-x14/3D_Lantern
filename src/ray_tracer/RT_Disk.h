#ifndef RT_DISK_H
#define RT_DISK_H
#include "RT_Object.h"
#include "RT_Material.h"

class RT_Disk : public RT_Object {
public:
    // inner_radius > 0 makes an annulus (useful for rings/washers)
    RT_Disk(const Point3d& _center, const Vec3d& _normal,
            double _radius, double _inner_radius,
            RT_Material* _material);

    RT_Disk(RT_Material* _material);
        
    void setTransform(const TRSTransformd& _transform) override;

    bool rayIntersect(const Rayd& ray, const Intervald& t_interval,
                      RT_Record& rec) const override;

    static void regressionTest();

private:
    Point3d center;
    Vec3d   normal;           // unit normal, precomputed
    double  radius;
    double  inner_radius;     // 0 for a solid disk
    double  D;                // plane constant, precomputed
    RT_Material* material;
};

#endif