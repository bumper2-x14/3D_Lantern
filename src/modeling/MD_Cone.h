#ifndef MD_CONE_H
#define MD_CONE_H

#include "MD_Shape.h"

/// @brief Represents a cone shape, generated procedurally using a number of segments.
class MD_Cone : public MD_Shape{
    public:
        /// @brief Creates a cone with a given number of segments.
        /// @param _nb_seg Number of segments used to approximate the base circle.
        MD_Cone(int _nb_seg = 25);

        /// @brief Builds the geometry (vertices + indices) of the cone.
        void buildShape() override;

        /// @brief Applies a transformation (translation, rotation, scale) to the cone.
        /// @param transform Pointer to the transform to apply.
        void applyTransform(Transform* transform) override; 

    private:
        int nb_seg; ///< Number of segments used to build the cone.
};

#endif