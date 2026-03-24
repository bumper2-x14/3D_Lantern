#include "RT_Box.h"

RT_Box::RT_Box(const Point3d& _min, const Point3d& _max, RT_Material* _material)
    : box_min(_min), box_max(_max), material(_material)
{
    Vec3d dx(_max.x - _min.x, 0, 0);
    Vec3d dy(0, _max.y - _min.y, 0);
    Vec3d dz(0, 0, _max.z - _min.z);

    Point3d pMMM(_min.x, _min.y, _min.z);  // all min
    Point3d pXMM(_max.x, _min.y, _min.z);  // x max
    Point3d pMXM(_min.x, _max.y, _min.z);  // y max
    Point3d pMMX(_min.x, _min.y, _max.z);  // z max

    
    faces[0] = RT_Quad(pMMX, dx, dy, material);  // FRONT  (+Z)
    faces[1] = RT_Quad(pMMM, dy, dx, material);  // BACK   (-Z)
    faces[2] = RT_Quad(pMMM, dz, dy, material);  // LEFT   (-X)
    faces[3] = RT_Quad(pXMM, dy, dz, material);  // RIGHT  (+X)
    faces[4] = RT_Quad(pMXM, dz, dx, material);  // TOP    (+Y)
    faces[5] = RT_Quad(pMMM, dx, dz, material);  // BOTTOM (-Y)
    
}
    

bool RT_Box::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    bool hit_any = false;
    RT_Record closest = rec;
    Intervald range = t_interval;
    
    for (const RT_Quad& face : faces) {
        RT_Record tmp;
        if (face.rayIntersect(ray, range, tmp)) {
            range = Intervald(t_interval.min, tmp.t);
            closest = tmp;
            hit_any = true;
        }
    }

    if (hit_any) rec = closest;
    return hit_any;
}