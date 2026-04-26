#ifndef MD_DISK_H
#define MD_DISK_H

#include "MD_Shape.h"
#include "assets/vertex.h"

/// @brief Represents a flat disk (circle) built using a set of segments.
class MD_Disk : public MD_Shape{
    public:
        /// @brief Creates a disk with a given number of segments.
        /// @param _nb_seg Number of segments used to approximate the circle.
        MD_Disk(int _nb_seg = 25);

        /// @brief Builds the disk geometry (vertices + indices).
        void buildShape() override;

        /// @brief Applies a transform (translation, rotation, scale) to the disk.
        /// @param transform Pointer to the transform to apply.
        void applyTransform(Transform* transform) override;
        
    private:
        int nb_seg; ///< Number of segments used to build the disk.

};

#endif