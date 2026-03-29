#ifndef RT_OBJECT_H
#define RT_OBJECT_H

#include "math/ray.h"
#include "math/interval.h"
#include "math/TRS_transform.h"
#include "RT_Record.h"


class RT_Object {
    public:
        virtual ~RT_Object() = default;    

        virtual bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const = 0;

        virtual void setTransform(const TRSTransformd& _transform) {
            tr = _transform; // Default - used by ray-transform primitives
        }
    
    protected:
        const Mat4d& getMatrix() const {
            return tr.getMatrix();
        }

        const Mat4d& getInverse() const {
            return tr.getInverse();
        }

        Rayd transformRay(const Rayd& ray, const Mat4d& inv) const {
            Vec3d localDir = inv * ray.getDirection();
            double scale = localDir.length();       // capture scale factor
            return Rayd(inv * ray.getOrigin(), localDir);            

        }

        Vec3d transformNormal(const Vec3d& n, const Mat4d& inv) const {
            return normalize(inv.transpose() * n);
        }

    private:
        TRSTransformd tr;

};

#endif
