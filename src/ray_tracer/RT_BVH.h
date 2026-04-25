#ifndef RT_BVH_H
#define RT_BVH_H

#include "math/bounding_box.h"
#include "RT_Object.h"
#include "RT_ObjectList.h"

/**
 * @class BVHNode
 * @brief Bounding Volume Hierarchy node used to accelerate ray-object intersection tests.
 *
 * The BVH recursively partitions objects into a binary tree structure,
 * reducing the number of intersection tests required during ray tracing.
 */
class BVHNode : public RT_Object {
    public:
        BVHNode();
        BVHNode(RT_ObjectList& _objects);
        BVHNode(RT_ObjectList* _objects);
        BVHNode(std::vector<RT_Object*>& _objects, size_t start, size_t end);
        ~BVHNode();

        /**
         * @brief Tests ray intersection against the BVH structure.
         *
         * Traverses the hierarchy and checks for intersections efficiently
         * using bounding boxes.
         *
         * @param ray Incoming ray.
         * @param t_interval Valid intersection interval.
         * @param rec Intersection record output.
         * @return true if an intersection is found.
         * @return false otherwise.
         */
        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;
    private:
        RT_Object* left;  ///< Pointer to the left child node or object.
        RT_Object* right;  ///< Pointer to the right child node or object.
        
        /**
         * @brief Compares two objects along a given axis using their bounding boxes.
         */
        static bool bboxCompare(const RT_Object* obj1, const RT_Object* obj2, int axis);

        static bool compareBox_x(const RT_Object* obj1, const RT_Object* obj2);
        static bool compareBox_y(const RT_Object* obj1, const RT_Object* obj2);
        static bool compareBox_z(const RT_Object* obj1, const RT_Object* obj2);
};

#endif