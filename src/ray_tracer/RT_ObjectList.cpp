#include <iostream>
#include "RT_ObjectList.h"

RT_ObjectList::RT_ObjectList(RT_Object* object) {
    add(object);
}

void RT_ObjectList::add(RT_Object* object) {
    objects.push_back(object);
}

void RT_ObjectList::clear() { objects.clear(); }      
bool RT_ObjectList::empty() const { return objects.empty(); }     
int RT_ObjectList::size() const { return objects.size(); }

        
bool RT_ObjectList::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    RT_Record rec_tmp;
    bool intersected = false; // Detect if there was an ray object intersection
    double closest = t_interval.max;

    for (const auto& obj : objects) {
        if (obj->rayIntersect(ray, Intervald(t_interval.min, closest), rec_tmp)) {
            intersected = true;
            closest = rec_tmp.t;
            rec = rec_tmp;
        }
    }

    return intersected;
} 

#include "ray_tracer/RT_Sphere.h"

void RT_ObjectList::regressionTest() {

    // Test default constructor
    RT_ObjectList list1;
    assert(list1.empty());
    assert(list1.size() == 0);

    // Test constructor with object
    RT_Sphere sphere1(Point3d(0, 0, -1), 0.5);
    RT_ObjectList list2(&sphere1);
    assert(!list2.empty());
    assert(list2.size() == 1);

    // Test add
    RT_Sphere sphere2(Point3d(0, -100.5, -1), 100);
    list2.add(&sphere2);
    assert(list2.size() == 2);

    // Test clear
    list2.clear();
    assert(list2.empty());
    assert(list2.size() == 0);

    // Test rayIntersect hits sphere
    RT_ObjectList list3;
    RT_Sphere sphere3(Point3d(0, 0, -1), 0.5);
    list3.add(&sphere3);
    Rayd ray(Point3d(0, 0, 0), Vec3d(0, 0, -1));
    RT_Record rec;
    bool hit = list3.rayIntersect(ray, Intervald(0.001, 100.0), rec);
    assert(hit);
    assert(rec.t > 0);

    // Test rayIntersect misses
    Rayd miss_ray(Point3d(0, 0, 0), Vec3d(0, 1, 0)); // pointing up, misses sphere
    RT_Record miss_rec;
    bool miss = list3.rayIntersect(miss_ray, Intervald(0.001, 100.0), miss_rec);
    assert(!miss);

    // Test closest hit is returned with two spheres
    RT_ObjectList list4;
    RT_Sphere near(Point3d(0, 0, -1), 0.5);
    RT_Sphere far(Point3d(0, 0, -3), 0.5);
    list4.add(&near);
    list4.add(&far);
    RT_Record closest_rec;
    list4.rayIntersect(ray, Intervald(0.001, 100.0), closest_rec);
    assert(closest_rec.t < 1.0); // should hit near sphere first

    std::cout << "All RT_ObjectList tests passed successfully\n";
}