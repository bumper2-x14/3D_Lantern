#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

#include "MD_Shape.h"
#include "MD_Mesh.h"
#include "assets/vertex.h"


class Sphere : public MD_Shape{
   public : 
      Sphere(float _longtitude_seg, float _latitude_seg);
      void buildShape() override;
      void applyTransform(Transform* transform) override;
      static void regressionTest();
      
   private : 

      float longitude_seg;
      float latitude_seg;

};
#endif 