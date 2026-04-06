#include "RT_Triangle.h"

RT_Triangle::RT_Triangle(const Vec3d& _v0, const Vec3d& _v1, const Vec3d& _v2,
                         const Vec3d& _n0, const Vec3d& _n1, const Vec3d& _n2,
                         const Vec2d& _uv0, const Vec2d& _uv1, const Vec2d& _uv2,
                         RT_Material* _material)
    : v0(_v0), v1(_v1), v2(_v2)
    , n0(_n0), n1(_n1), n2(_n2)
    , uv0(_uv0), uv1(_uv1), uv2(_uv2)
    , material(_material) {}

void RT_Triangle::setTransform(const TRSTransformd& t) {
    RT_Object::setTransform(t);
    setBoundingBox(computeBBox(computeLocalBBox()));
}

BoundingBoxd RT_Triangle::computeLocalBBox() const {
    return BoundingBoxd(
        Intervald(std::min({v0.x, v1.x, v2.x}), std::max({v0.x, v1.x, v2.x})),
        Intervald(std::min({v0.y, v1.y, v2.y}), std::max({v0.y, v1.y, v2.y})),
        Intervald(std::min({v0.z, v1.z, v2.z}), std::max({v0.z, v1.z, v2.z}))
    );
}

bool RT_Triangle::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    // Following Möller–Trumbore algorithm for ray triangle intersection
    const double EPSILON = 1e-8;
    Mat4d inv = getInverse();
    Rayd local_ray = transformRay(ray, inv);

    Vec3d edge1 = v1 - v0;
    Vec3d edge2 = v2 - v0;

    Vec3d h = cross(local_ray.getDirection(), edge2);
    double det = dot(edge1, h);
    if (std::abs(det) < EPSILON) return false;

    double inv_det = 1.0 / det;
    Vec3d s = Vec3d(local_ray.getOrigin()) - v0;
    double u = inv_det * dot(s, h);
    if (u < 0.0 || u > 1.0) return false;

    Vec3d q = cross(s, edge1);
    double v = inv_det * dot(local_ray.getDirection(), q);
    if (v < 0.0 || u + v > 1.0) return false;

    double t = inv_det * dot(edge2, q);
    if (!t_interval.surrounds(t)) return false;

    double w = 1.0 - u - v;
    rec.t = t;
    rec.p = getMatrix() * local_ray.at(t);
    rec.normal = transformNormal(normalize(w * n0 + u * n1 + v * n2), inv);
    rec.uv = w * uv0 + u * uv1 + v * uv2;
    rec.setNormal(ray, rec.normal);
    rec.material = material;
    return true;
}