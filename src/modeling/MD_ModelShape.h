#ifndef MD_MODELSHAPE_H
#define MD_MODELSHAPE_H

#include "MD_Shape.h"
#include "assets/mesh_data.h"
#include "assets/model.h" 

/// @brief Shape that wraps an already existing mesh (usually loaded from a model file).
///        Instead of generating geometry, it uses provided mesh data.
class MD_ModelShape: public MD_Shape {
    public:
        /// @brief Creates a shape from existing mesh data.
        /// @param _mesh_data Pointer to mesh data to use.
        MD_ModelShape(MeshData* _mesh_data);

        /// @brief Builds the shape (basically assigns the mesh data to the internal mesh).
        void buildShape() override;

        /// @brief Applies a transform (translation, rotation, scale) to the mesh.
        /// @param transform Pointer to the transform to apply.
        void applyTransform(Transform* transform) override;

    private:
       std::string path;   ///< Optional path to the original model file (if used).
       MeshData* meshData = nullptr; ///< Pointer to the mesh data used by this shape.
};

#endif