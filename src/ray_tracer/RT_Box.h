#ifndef RT_BOX_H
#define RT_BOX_H
#include "RT_Object.h"
#include "RT_Quad.h"
#include "RT_Material.h"

class RT_Box : public RT_Object {
public:
    // Axis-aligned box defined by two opposite corners
    RT_Box(RT_Material* _material);

    void setTransform(const TRSTransformd& _transfom) override;

    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

    static void regressionTest();

private:
    RT_Quad faces[6];  // 6 RT_Quad instances
    RT_Material* material;
};

#endif