#ifndef RT_OBJECTLIST_H
#define RT_OBJECTLIST_H

#include <vector>

#include "RT_Object.h"

class RT_ObjectList : public RT_Object {
    public:
        RT_ObjectList() = default;
        RT_ObjectList(RT_Object* object);

        void add(RT_Object* object);
        void clear();
        bool empty() const;
        int size() const;

        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec)const override;  
        
    private:
        std::vector<RT_Object*> objects;
};


#endif