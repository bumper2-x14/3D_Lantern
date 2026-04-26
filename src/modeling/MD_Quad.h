#ifndef MD_QUAD_H
#define MD_QUAD_H

#include "MD_Shape.h"
#include "assets/vertex.h"

/// @brief Represents a simple quad (rectangle) in 3D space.
///        Defined by a width and a length.
class MD_Quad : public MD_Shape{
    public:
        /// @brief Creates a quad with given dimensions.
        /// @param _width Width of the quad.
        /// @param _lenght Length of the quad.
        MD_Quad(float _width =1 , float _lenght = 1);

        /// @brief Builds the quad geometry (vertices + indices).
        void buildShape() override;

        /// @brief Applies a transform (translation, rotation, scale) to the quad.
        /// @param transform Pointer to the transform to apply.
        void applyTransform(Transform* transform) override;

    private:
        float width;  ///< Width of the quad.
        float length; ///< Length of the quad.    
};

#endif