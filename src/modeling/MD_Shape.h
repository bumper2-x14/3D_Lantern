#ifndef MD_SHAPE_H
#define MD_SHAPE_H

#include "math/transform.h"

class MD_Shape {
    public: 
        virtual void buildShape();
        virtual void applyTransform(Transform* transform);
};

#endif
