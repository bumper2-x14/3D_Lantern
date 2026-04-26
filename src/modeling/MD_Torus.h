#ifndef MD_TORUS_H
#define MD_TORUS_H

#include "MD_Shape.h"

/// @brief Represents a torus (donut shape) generated procedurally.
///        Built using two levels of subdivision: rings and sides.
class MD_Torus : public MD_Shape{
    public:
        /// @brief Creates a torus with given segmentation.
        /// @param _nb_ring Number of divisions around the main (big) circle.
        /// @param _nb_side Number of divisions around the tube (small circle).
        MD_Torus(int _nb_ring = 25, int _nb_side=25);

        /// @brief Builds the torus geometry (vertices + indices).
        void buildShape() override;

        /// @brief Applies a transform (translation, rotation, scale) to the torus.
        /// @param transform Pointer to the transform to apply.
        void applyTransform(Transform* transform) override; 

    private:
        int nb_ring; ///< Divisions around the big circle.
        int nb_side; ///< Divisions around the small circle (tube).
};

#endif