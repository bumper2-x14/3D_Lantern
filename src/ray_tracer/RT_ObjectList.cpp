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