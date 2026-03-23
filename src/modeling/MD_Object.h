#ifndef MD_OBJECT_H
#define MD_OBJECT_H

#include "MD_Shape.h"
#include "math/transform.h"

class MD_Object{
    public:
        MD_Object();
        MD_Object(MD_Shape* _shape, Transform* _transform);
        void draw(unsigned int shaderID);
    private:
        MD_Shape* shape;
        Transform* transform;
};

#endif
