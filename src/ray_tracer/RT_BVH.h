#ifndef RT_BVH_H
#define RT_BVH_H

#include "math/bounding_box.h"
#include "RT_Object.h"
#include "RT_ObjectList.h"

class BVHNode : public RT_Object {
    public:
        BVHNode();
        BVHNode(RT_ObjectList& _objects);
        BVHNode(RT_ObjectList* _objects);
        BVHNode(std::vector<RT_Object*>& _objects, size_t start, size_t end);
        ~BVHNode();

        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;
    private:
        RT_Object* left;
        RT_Object* right;

        static bool bboxCompare(const RT_Object* obj1, const RT_Object* obj2, int axis);
        static bool compareBox_x(const RT_Object* obj1, const RT_Object* obj2);
        static bool compareBox_y(const RT_Object* obj1, const RT_Object* obj2);
        static bool compareBox_z(const RT_Object* obj1, const RT_Object* obj2);
};

#endif