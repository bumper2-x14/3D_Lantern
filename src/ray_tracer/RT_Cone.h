#ifndef RT_CONE_H
#define RT_CONE_H

#include "RT_Object.h"

class RT_Cone : public RT_Object {
    public:
        RT_Cone(bool _capped, RT_Material* _material);

        void setTransform(const TRSTransformd& t) override;

        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

        static void regressionTest();

private:
        bool    capped;
        RT_Material* material;
};

#endif