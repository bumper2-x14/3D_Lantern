#ifndef MD_BOX_H
#define MD_BOX_H

#include "MD_Shape.h"
#include "assets/vertex.h"

/// @brief Represents a cube shape built procedurally.
///        Inherits from MD_Shape and defines its own geometry.
class MD_Box : public MD_Shape{
    public:
        /// @brief Creates a cube with a given size.
        /// @param _size Length of the cube edges.
        MD_Box(float _size = 1.0f);

        /// @brief Builds the cube geometry (vertices + indices).
        void buildShape() override;

        /// @brief Applies a transform (translation, rotation, scale) to the cube.
        /// @param transform Pointer to the transform to apply.
        void applyTransform(Transform* transform) override;

    private:
        float size; ///< Size of the cube (edge length).

        /// @brief Adds one face (quad) to the cube mesh.
        /// @param p0 First vertex of the face.
        /// @param p1 Second vertex of the face.
        /// @param p2 Third vertex of the face.
        /// @param p3 Fourth vertex of the face.
        /// @param normal Normal vector of the face.
        void addFace(Vec3f p0, Vec3f p1, Vec3f p2, Vec3f p3, Vec3f normal);
};

#endif