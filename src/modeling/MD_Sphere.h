#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

#include "MD_Mesh.h"
#include "assets/vertex.h"

class Sphere{
   public : 
      Sphere(float _longtitude_seg, float _latitude_seg);
      void buildSphere();

   private : 
      MD_Mesh mesh;
      float longitude_seg;
      float latitude_seg;

};
#endif 