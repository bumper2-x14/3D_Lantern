#include "RT_Box.h"

RT_Box::RT_Box(RT_Material* _material)
    : material(_material)
{
    // unit box from (-1,-1,-1) to (1,1,1)
    Point3d pMMM(-1,-1,-1);
    Point3d pXMM( 1,-1,-1);
    Point3d pMXM(-1, 1,-1);
    Point3d pMMX(-1,-1, 1);
    Vec3d dx(2,0,0);
    Vec3d dy(0,2,0);
    Vec3d dz(0,0,2);

    faces[0] = RT_Quad(pMMX, dx, dy, material);  // FRONT  (+Z)
    faces[1] = RT_Quad(pMMM, dy, dx, material);  // BACK   (-Z)
    faces[2] = RT_Quad(pMMM, dz, dy, material);  // LEFT   (-X)
    faces[3] = RT_Quad(pXMM, dy, dz, material);  // RIGHT  (+X)
    faces[4] = RT_Quad(pMXM, dz, dx, material);  // TOP    (+Y)
    faces[5] = RT_Quad(pMMM, dx, dz, material);  // BOTTOM (-Y)
    
}


void RT_Box::setTransform(const TRSTransformd& _transform) {
    RT_Object::setTransform(_transform);
    // propagate transform to each face
    for (RT_Quad& face : faces)
        face.setTransform(_transform);
    
    setBoundingBox(computeBBox(BoundingBoxd(Point3d(-1,-1,-1), Point3d(1,1,1))));
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