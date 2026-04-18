#ifndef MD_SHAPE_H
#define MD_SHAPE_H

#include "math/transform.h"
#include "MD_Mesh.h"

enum class ShapeType{
    SPHERE, CYLINDER, CONE, DISK, QUAD, MESH
};

class MD_Shape {
    public: 
        MD_Shape() : mesh(nullptr){}
        ~MD_Shape(){ delete mesh; } 

        virtual void buildShape() = 0;
        virtual void applyTransform(Transform* transform) = 0;
        virtual std::string serialize() const = 0;

        MD_Shape(const MD_Shape&) = delete;
        MD_Shape& operator=(const MD_Shape&) = delete;

        const MD_Mesh* getMesh()const { return mesh; }
        MD_Mesh* getMesh() {return mesh; }
   
    protected:
        MD_Mesh* mesh;
        ShapeType type;
        
};

#endif
