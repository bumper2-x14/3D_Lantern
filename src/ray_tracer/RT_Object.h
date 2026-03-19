#ifndef RT_OBJECT_H
#define RT_OBJECT_H

#include "math/ray.h"
#include "math/interval.h"
#include "RT_Record.h"

class RT_Object {
    public:
        virtual ~RT_Object() = default;    

        virtual bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const = 0;
};

#endif
