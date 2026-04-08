#ifndef RT_CYLINDER_H
#define RT_CYLINDER_H
#include "RT_Object.h"
#include "RT_Material.h"

class RT_Cylinder : public RT_Object {
public:
    RT_Cylinder(bool _capped, RT_Material* _material);

    void setTransform(const TRSTransformd& t) override;

    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

    static void regressionTest();

private:
    bool capped; // include top/bottom disk caps
    RT_Material* material;
};
#endif
