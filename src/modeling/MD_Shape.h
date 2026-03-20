#ifndef MD_SHAPE_H
#define MD_SHAPE_H

#include "math/transform.h"
#include "MD_Mesh.h"

class MD_Shape {
    public: 
        virtual void buildShape() = 0;
        virtual void applyTransform(Transform* transform) = 0;
        const MD_Mesh& getMesh()const { return mesh; }

    protected:
        MD_Mesh mesh;
        
};

#endif
