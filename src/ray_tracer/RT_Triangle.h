#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include "RT_Object.h"
#include "RT_Material.h"

class RT_Triangle : public RT_Object {
public:
    RT_Triangle(const Vec3d& v0, const Vec3d& v1, const Vec3d& v2,
                const Vec3d& n0, const Vec3d& n1, const Vec3d& n2,
                const Vec2d& uv0, const Vec2d& uv1, const Vec2d& uv2,
                RT_Material* material);

    void setTransform(const TRSTransformd& t) override;
    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

private:
    Vec3d v0, v1, v2;
    Vec3d n0, n1, n2;
    Vec2d uv0, uv1, uv2;
    RT_Material* material;

    BoundingBoxd computeLocalBBox() const;
};
#endif