#include "RT_Quad.h"
#include <cmath>

RT_Quad::RT_Quad() {}

// Q + u + v constructor — preferred
RT_Quad::RT_Quad(const Point3d& _Q, const Vec3d& _u, const Vec3d& _v,
                 RT_Material* _material)
    : Q(_Q), u(_u), v(_v), material(_material)
{
    Vec3d n = cross(u, v);
    normal = normalize(n);
    w = n / dot(n, n); // used for hit-point-in-quad test
    D = dot(normal, Vec3d(Q.x, Q.y, Q.z));
}


RT_Quad::RT_Quad(const Point3d& _point, const Vec3d& _normal,
                 RT_Material* _material)
    : Q(_point), material(_material)
{
    normal = normalize(_normal);
    D      = dot(normal, Vec3d(Q.x, Q.y, Q.z));

    // Build arbitrary u/v axes perpendicular to normal
    // so the bounded rayIntersect still works
    Vec3d up = std::fabs(normal.y) < 0.9 ? Vec3d(0,1,0) : Vec3d(1,0,0);
    u = normalize(cross(up, normal)) * 1e6;  // effectively infinite quad
    v = cross(normal, normalize(u))  * 1e6;
    w = normalize(normal);
}


bool RT_Quad::rayIntersect(const Rayd& ray, const Intervald& t_interval,
                           RT_Record& rec) const
{
    double denom = dot(normal, ray.getDirection());
    if (std::fabs(denom) < 1e-8)
        return false;   // parallel

    double t = (D - dot(normal, Vec3d(ray.getOrigin().x,
                                      ray.getOrigin().y,
                                      ray.getOrigin().z))) / denom;
    if (!t_interval.contains(t))
        return false;

    // Check hit point is within the parallelogram
    Point3d p = ray.at(t);
    Vec3d hit = p - Q;
    double alpha = dot(w, cross(hit, v));
    double beta = dot(w, cross(u, hit));

    if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1)
        return false;   // outside quad bounds

    rec.t = t;
    rec.p = p;
    rec.material = material;
    rec.setNormal(ray, normal);
    rec.uv.x = alpha;
    rec.uv.y = beta;

    return true;
}

void RT_Quad::regressionTest() { /* TODO */ }