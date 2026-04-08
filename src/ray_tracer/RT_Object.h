#ifndef RT_OBJECT_H
#define RT_OBJECT_H

#include "math/ray.h"
#include "math/interval.h"
#include "math/TRS_transform.h"
#include "math/bounding_box.h"
#include "RT_Record.h"


class RT_Object {
    public:
        virtual ~RT_Object() = default;    

        virtual bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const = 0;

        virtual void setTransform(const TRSTransformd& _transform) {
            tr = _transform; // Default - used by ray-transform primitives 
        }

        BoundingBoxd getBoundingBox() const {
            return bbox;
        }

        BoundingBoxd& getBoundingBox() {
            return bbox;
        }

        TRSTransformd getTransform() const {
            return tr;
        }

        const Mat4d& getMatrix() const {
            return tr.getMatrix();
        }

        const Mat4d& getInverse() const {
            return tr.getInverse();
        }
    
    protected:
        Rayd transformRay(const Rayd& ray, const Mat4d& inv) const {
            Vec3d localDir = inv * ray.getDirection();
            double scale = localDir.length();       // capture scale factor
            return Rayd(inv * ray.getOrigin(), localDir);            

        }

        Vec3d transformNormal(const Vec3d& n, const Mat4d& inv) const {
            return normalize(inv.transpose() * n);
        }

        void setBoundingBox(const BoundingBoxd& _bbox) {
            bbox = _bbox;
        }

        BoundingBoxd computeBBox (const BoundingBoxd& local_box) const {
            const Mat4d& M = getMatrix();
            // 8 corners of the local bbox
            Point3d corners[8] = {
                Point3d(local_box.x.min, local_box.y.min, local_box.z.min),
                Point3d(local_box.x.max, local_box.y.min, local_box.z.min),
                Point3d(local_box.x.min, local_box.y.max, local_box.z.min),
                Point3d(local_box.x.max, local_box.y.max, local_box.z.min),
                Point3d(local_box.x.min, local_box.y.min, local_box.z.max),
                Point3d(local_box.x.max, local_box.y.min, local_box.z.max),
                Point3d(local_box.x.min, local_box.y.max, local_box.z.max),
                Point3d(local_box.x.max, local_box.y.max, local_box.z.max),
            };

            // transform each corner and find tight bounds
            Point3d first = M * corners[0];
            double xmin = first.x, xmax = first.x;
            double ymin = first.y, ymax = first.y;
            double zmin = first.z, zmax = first.z;

            for (int i = 1; i < 8; i++) {
                Point3d p = M * corners[i];
                xmin = std::min(xmin, p.x); xmax = std::max(xmax, p.x);
                ymin = std::min(ymin, p.y); ymax = std::max(ymax, p.y);
                zmin = std::min(zmin, p.z); zmax = std::max(zmax, p.z);
            }

            return BoundingBoxd(Intervald(xmin, xmax), Intervald(ymin, ymax), Intervald(zmin, zmax));
        }

    private:
        TRSTransformd tr;
        BoundingBoxd bbox = BoundingBoxd::empty;

};

#endif
