#include <algorithm>

#include "RT_BVH.h"

BVHNode::BVHNode() : left(nullptr), right(nullptr) {
    setBoundingBox(BoundingBoxd::empty);
}

BVHNode::BVHNode(RT_ObjectList& _objects) : BVHNode(_objects.objects, 0, _objects.size()) {}

BVHNode::BVHNode(RT_ObjectList* _objects) : BVHNode(_objects->objects, 0, _objects->size()) {}

BVHNode::BVHNode(std::vector<RT_Object*>& _objects, size_t start, size_t end) {
    BoundingBoxd box = BoundingBoxd::empty;
    for (size_t i = start; i < end; i++){
        box = BoundingBoxd(box, _objects[i]->getBoundingBox());
    }
    setBoundingBox(box);

    int ax = box.longestAxis();
    auto lambdaCmp = (ax == 0) ? compareBox_x :
                     (ax == 1) ? compareBox_y  :
                                 compareBox_z;
    size_t span = end - start;
    if (span == 1) {
        left = right = _objects[start];
    }
    else if (span == 2) {
        left = _objects[start];
        right = _objects[start+1];
    }
    else {
        std::sort(std::begin(_objects) + start, std::begin(_objects) + end, lambdaCmp);
        size_t mid = start + span/2;
        left = new BVHNode(_objects, start, mid);
        right = new BVHNode(_objects, mid, end);
    }
}

BVHNode::~BVHNode() {
    // only delete children that are BVHNodes, not leaf RT_Objects we don't own
    if (left == right) {
        // span == 1 case, we don't own this
    } else {
        if (dynamic_cast<BVHNode*>(left))  delete left;
        if (dynamic_cast<BVHNode*>(right)) delete right;
    }
    left = right = nullptr;
}

bool BVHNode::rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const {
    Intervald t = t_interval;
    if (!getBoundingBox().rayIntersect(ray, t)) {
        return false;
    }

    bool intersect_left = left->rayIntersect(ray, t_interval, rec);
    bool intersect_right = right->rayIntersect(ray, Intervald(t_interval.min, 
                                                        intersect_left ? rec.t : t_interval.max), rec);

    return intersect_left || intersect_right; 
}

bool BVHNode::bboxCompare(const RT_Object* obj1, const RT_Object* obj2, int axis) {
    Intervald obj1_interval = obj1->getBoundingBox()[axis];
    Intervald obj2_interval = obj2->getBoundingBox()[axis];
    return obj1_interval.min < obj2_interval.min;
}

bool BVHNode::compareBox_x(const RT_Object* obj1, const RT_Object* obj2) {
    return bboxCompare(obj1, obj2, 0);
}

bool BVHNode::compareBox_y(const RT_Object* obj1, const RT_Object* obj2) {
    return bboxCompare(obj1, obj2, 1);
}

bool BVHNode::compareBox_z(const RT_Object* obj1, const RT_Object* obj2){
    return bboxCompare(obj1, obj2, 2);
}