#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

#include "MD_Mesh.h"
#include "assets/vertex.h"

class Sphere{
   public : 
      Sphere();
      void build_sphere();

   private : 
      MD_Mesh mesh;
        
};
#endif 