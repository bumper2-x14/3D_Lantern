#ifndef MD_SHAPE_H
#define MD_SHAPE_H

#include "math/transform.h"
#include "MD_Mesh.h"

/// @brief Enum describing the type of shape.
enum class ShapeType{
    BOX, SPHERE, CYLINDER, CONE, DISK, QUAD, MESH
};

/// @brief Base class for all shapes.
///        Defines common interface (build + transform) and stores a mesh.
class MD_Shape {
    public: 
        ShapeType type; ///< Type of the shape.

        /// @brief Default constructor (no mesh yet).
        MD_Shape() : mesh(nullptr){}

        /// @brief Destructor (frees mesh if allocated).
        ~MD_Shape(){ delete mesh; } 

        /// @brief Builds the geometry of the shape (vertices + indices).
        virtual void buildShape() = 0;

        /// @brief Applies a transform (translation, rotation, scale) to the shape.
        virtual void applyTransform(Transform* transform) = 0;

        // disable copy (shapes own their mesh)
        MD_Shape(const MD_Shape&) = delete;
        MD_Shape& operator=(const MD_Shape&) = delete;

        /// @brief Returns const pointer to mesh.
        const MD_Mesh* getMesh()const { return mesh; }

        /// @brief Returns mutable pointer to mesh.
        MD_Mesh* getMesh() {return mesh; }

        /// @brief Converts shape type enum to string.
        std::string typeToString(ShapeType type) const{
            switch (type) {
                case ShapeType::BOX : return "box"; break;
                case ShapeType:: SPHERE : return "sphere"; break;
                case ShapeType:: CYLINDER : return "cylinder"; break;
                case ShapeType:: CONE : return "CONE"; break;
                case ShapeType:: DISK : return "DISK"; break;
                case ShapeType:: QUAD : return "quad"; break;
                case ShapeType:: MESH : return "mesh"; break;
            }
            return "";
        } 
        
    protected:
        MD_Mesh* mesh; ///< Pointer to the mesh representing this shape.
};

#endif