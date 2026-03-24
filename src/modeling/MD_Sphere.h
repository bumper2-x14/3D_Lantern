#ifndef MD_SPHERE_H
#define MD_SPHERE_H

#include <vector>

#include "MD_Shape.h"
#include "MD_Mesh.h"
#include "assets/vertex.h"


class MD_Sphere : public MD_Shape{
   public : 
      MD_Sphere(float _longtitude_seg = 25, float _latitude_seg = 25);
      void buildShape() override;
      void applyTransform(Transform* transform) override;
      static void regressionTest();
      
   private : 

      float longitude_seg;
      float latitude_seg;

};
#endif 
