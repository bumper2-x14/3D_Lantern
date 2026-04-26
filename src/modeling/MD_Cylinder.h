#ifndef MD_CYLINDER_H
#define MD_CYLINDER_H

#include "MD_Shape.h"
#include "assets/vertex.h"

/// @brief Represents a cylinder shape built procedurally.
///        The geometry is approximated using a given number of segments.
class MD_Cylinder : public MD_Shape{
    public:
        /// @brief Creates a cylinder with a given number of segments.
        /// @param _nb_seg Number of segments used to approximate the circular base.
        MD_Cylinder(int _nb_seg = 25);

        /// @brief Builds the cylinder geometry (vertices + indices).
        void buildShape() override;

        /// @brief Applies a transform (translation, rotation, scale) to the cylinder.
        /// @param transform Pointer to the transform to apply.
        void applyTransform(Transform* transform) override; 

    private:
        int nb_seg; ///< Number of segments used to build the cylinder.
};

#endif