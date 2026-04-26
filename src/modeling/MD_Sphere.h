#ifndef MD_SPHERE_H
#define MD_SPHERE_H

#include <vector>

#include "MD_Shape.h"
#include "MD_Mesh.h"
#include "assets/vertex.h"

/// @brief Represents a sphere shape generated using latitude/longitude segmentation.
class MD_Sphere : public MD_Shape{
   public : 
      /// @brief Creates a sphere with given segmentation.
      /// @param _longtitude_seg Number of segments around the sphere (horizontal).
      /// @param _latitude_seg Number of segments from top to bottom (vertical).
      MD_Sphere(float _longtitude_seg = 25, float _latitude_seg = 25);

      /// @brief Builds the sphere geometry (vertices + indices).
      void buildShape() override;

      /// @brief Applies a transform (translation, rotation, scale) to the sphere.
      /// @param transform Pointer to the transform to apply.
      void applyTransform(Transform* transform) override;

      /// @brief Runs basic tests to validate sphere generation.
      static void regressionTest();
      
   private : 

      float longitude_seg; ///< Number of horizontal segments.
      float latitude_seg;  ///< Number of vertical segments.

};
#endif 