#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "interval.h"
#include "point3.h"
#include "ray.h"

template <typename  T> class BoundingBox {
    public:
        Interval<T> x, y, z;

        BoundingBox() : x(Interval<T>::empty), y(Interval<T>::empty), z(Interval<T>::empty) {}

        BoundingBox(const Interval<T>& _x, const Interval<T>& _y, const Interval<T>& _z) :
            x(_x), y(_y), z(_z) {
                
                minPadding();
            }

        BoundingBox(const Point3<T>& p1, const Point3<T>& p2) {
            x = (p1.x <= p2.x) ? Interval<T>(p1.x, p2.x) : Interval<T>(p2.x, p1.x);
            y = (p1.y <= p2.y) ? Interval<T>(p1.y, p2.y) : Interval<T>(p2.y, p1.y);
            z = (p1.z <= p2.z) ? Interval<T>(p1.z, p2.z) : Interval<T>(p2.z, p1.z);  

            minPadding();
        }

        BoundingBox(const BoundingBox<T>& bbox1, const BoundingBox<T>& bbox2) {
            x = Interval<T>(bbox1.x, bbox2.x);
            y = Interval<T>(bbox1.y, bbox2.y);
            z = Interval<T>(bbox1.z, bbox2.z);
        }

        Interval<T>& operator[](int i) {
            assert(i >= 0 && i < 3);
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        const Interval<T>& operator[](int i) const {
            assert(i >= 0 && i < 3);
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        bool rayIntersect(const Ray<T>& ray, Interval<T>& t_val) const {
            const Point3<T>& r_origin = ray.getOrigin();
            const Vec3<T>& r_direction = ray.getDirection();

            for (int ax = 0; ax < 3; ax++){
                // Going through each axis to find the entry and exit points and detect intersections if any
                Interval<T> axis = (*this)[ax];
                T inv_dir = T(1) / r_direction[ax];
                
                T t_entry = (axis.min - r_origin[ax]) * inv_dir;
                T t_exit = (axis.max - r_origin[ax]) * inv_dir;

                if (t_entry < t_exit) {
                    if (t_entry > t_val.min) t_val.min = t_entry;
                    if (t_exit < t_val.max) t_val.max = t_exit;
                
                }else {
                    if (t_exit > t_val.min) t_val.min = t_exit;
                    if (t_entry < t_val.max) t_val.max = t_entry;
                }

                if (t_val.min >= t_val.max) return false;
            }

            return true;
        }

        int longestAxis() const {
            if (x.size() > y.size() && x.size() > z.size()) return 0;
            if (y.size() > z.size()) return 1;
            return 2;
        }

        static const BoundingBox<T> empty, world;

    private:
        void minPadding() {
            // Adjust the Bounding Box so that no side is narrower than some delta, padding if necessary.
            double delta = 0.0001;
            if (x.size() < delta) x = x.expand(delta);
            if (y.size() < delta) y = y.expand(delta);
            if (z.size() < delta) z = z.expand(delta);
        }
};

using BoundingBoxf = BoundingBox<float>;
using BoundingBoxd = BoundingBox<double>;

template <typename T>
const BoundingBox<T> BoundingBox<T>::empty = BoundingBox<T>(Interval<T>::empty, Interval<T>::empty, Interval<T>::empty);
template <typename T>

const BoundingBox<T> BoundingBox<T>::world = BoundingBox<T>(Interval<T>::world, Interval<T>::world, Interval<T>::world);

template <typename T>
BoundingBox<T> operator+(const BoundingBox<T>& bbox, const Vec3<T>& offset){
    return BoundingBox<T>(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
}

template <typename T>
BoundingBox<T> operator+(const Vec3<T>& offset, const BoundingBox<T>& bbox) {
    return bbox + offset;
}

#endif