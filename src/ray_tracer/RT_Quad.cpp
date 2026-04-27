#include "RT_Quad.h"
#include <cmath>

RT_Quad::RT_Quad() {}

// Q + u + v constructor — preferred
RT_Quad::RT_Quad(const Point3d& _Q, const Vec3d& _u, const Vec3d& _v, RT_Material* _material)
    : Q0(_Q), u0(_u), v0(_v),  // add these
      Q(_Q),  u(_u),  v(_v),
      material(_material) {
    Vec3d n = cross(u, v);
    normal  = normalize(n);
    w = n / dot(n, n);
    D = dot(normal, Vec3d(Q.x, Q.y, Q.z));
}

RT_Quad::RT_Quad(RT_Material* _material) : 
    Q0(Point3d(-0.5, -0.5, 0)),
    u0(Vec3d(1, 0, 0)),
    v0(Vec3d(0, 1, 0)),
    Q (Point3d(-0.5, -0.5, 0)),
    u (Vec3d(1, 0, 0)),
    v (Vec3d(0, 1, 0)),
    material(_material)
{
    Vec3d n = cross(u, v);
    normal = normalize(n);
    w = n / dot(n, n);
    D = dot(normal, Vec3d(Q.x, Q.y, Q.z));
}


void RT_Quad::setTransform(const TRSTransformd& _transform) {
    RT_Object::setTransform(_transform);
    const Mat4d& M = getMatrix();
    // always transform from canonical values — not from previously transformed ones
    Q = M * Q0;   // point — translation applies
    u = M * u0;   // direction — no translation (Vec3 operator)
    v = M * v0;   // direction — no translation (Vec3 operator)

    Vec3d n = cross(u, v);
    normal = normalize(n);
    w = n / dot(n, n);
    D = dot(normal, Vec3d(Q.x, Q.y, Q.z));
    
    // 4 corners of the quad — no hardcoding needed
    Point3d c0 = Q;
    Point3d c1 = Q + u;
    Point3d c2 = Q + v;
    Point3d c3 = Q + u + v;

    setBoundingBox(BoundingBoxd(
        Point3d(std::min({c0.x, c1.x, c2.x, c3.x}), std::min({c0.y, c1.y, c2.y, c3.y}), std::min({c0.z, c1.z, c2.z, c3.z})),
        Point3d(std::max({c0.x, c1.x, c2.x, c3.x}), std::max({c0.y, c1.y, c2.y, c3.y}), std::max({c0.z, c1.z, c2.z, c3.z}))
    ));
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