#ifndef RT_QUAD_H
#define RT_QUAD_H
#include "RT_Object.h"
#include "RT_Material.h"

class RT_Quad : public RT_Object {
public:
    RT_Quad();
    // Q = corner point, u/v = edge vectors
    RT_Quad(const Point3d& _Q, const Vec3d& _u, const Vec3d& _v, RT_Material* _material);

    // Legacy: point + normal (infinite plane)
    RT_Quad(const Point3d& _point, const Vec3d& _normal,
            RT_Material* _material);

    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

    static void regressionTest();

private:
    Point3d Q;
    Vec3d   u, v;
    Vec3d   normal;   // precomputed: normalize(cross(u, v))
    Vec3d   w;        // precomputed: normal / dot(normal, normal)
    double  D;        // plane constant
    RT_Material* material;
};
#endif